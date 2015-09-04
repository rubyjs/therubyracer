class V8::Conversion
  ##
  # The identity conversion is what ensures that a Ruby object is
  # represented by the exact same JavaScript object whenever it is
  # embedded into a given context, and that the same JavaScript object
  # is always represented by the same Ruby object when it is brought
  # over into Ruby.
  #
  # this arises when we embed objects into things like:
  #
  #   cxt = V8::Context.new
  #   object = Object.new
  #   cxt['rubyObject'] = object
  #   cxt['sameObject] = object
  #
  # We want to ensure that all of the following cases hold:
  #
  #   # unwraps the same instance back to Ruby
  #   cxt['rubyObject'] == cxt['rubyObject'] #=> true
  #   cxt['rubyObject'] == cxt['sameObject] #=> true
  #
  #   # same js object:
  #   cxt.eval('rubyObject === sameObject') #=> true
  #   cxt.eval('var jsObject = new Object()')
  #   cxt['jsObject'] == cxt['jsObject'] #=> true
  #
  # Let's first talk about the case when we are embedding a Ruby
  # object into JavaScript. That is to say, the object was *allocated*
  # by Ruby, and the *peer* object will be in JavaScript. We denote
  # this, by making the R capital to indicate that it hails from Ruby land.
  #
  #   R --> j
  #
  # In order to make sure than whenever we embed `R` into JavaScript,
  # we insert `j`, we have to store the value in a map somewhere such
  # that M[R] -> j always.
  #
  # This must be implemented subtley in the code because of two
  # things: 1) there could be no more references to R from ruby once
  # it is embedded into JavaScript, so it is critical for `R` never
  # to be garbage collected while `j` is still around in
  # JavaScript. Otherwise, `R` might come back to ruby either as a
  # return value of a javascript function, or as the parameter to a
  # method call. By the same token, we don't want the fact that `j` is
  # in our identity map to prevent it from being garbage collected by
  # JavaScript.
  #
  # This means that we must hold a strong reference to neither the
  # Ruby object nor the JavaScript object, otherwise, the mapping
  # would prevent the objects from being garbage collected
  # forever. Not good!
  #
  # For the Ruby object, this means that we only ever store it by id
  # in the identity map. The JavaScript object on the other hand, we
  # need to store as an object since ultimately that's what we want to
  # look up when deciding what object to pass to javascript. This
  # reference then, must be a weak reference. Now, if we store both
  # the Ruby object by id, and a weak reference to the v8 object, then
  # the fact that these objects are mapped has no effect on their
  # garbage collection.
  module Identity

    def to_v8(context, ruby_object)
      object_id = ruby_object.object_id
      peer = v8_peers[object_id]

      # this holds a strong reference to the ruby object from JavaScript because `peer` is a weak reference. That way if peer holds on to
      handle = peer.nil? ? nil : V8::C::Handle::New(peer)
      if !handle || handle.IsEmpty()
        peer = v8_peers[object_id] = super
        if peer.kind_of? V8::C::Object
          wrapper = V8::C::External::New(peer_content_key, ruby_object)
          peer.SetHiddenValue(symbol, wrapper)
        end
        cleanup = self.class.cleanup_peer_mapping(v8_peers, object_id, peer)
        peer.SetWeak(cleanup)
      else
        return handle
      end
    end

    def to_ruby(context, v8_object)
      context.isolate.run_v8_finalizers!
      if v8_object.kind_of? V8::C::Object
        object_id = v8_object.GetIdentityHash()
        if wrapper = v8_object.GetHiddenValue(peer_content_key)
          # this was allocated in Ruby, and we have a JavaScript peer
          return wrapper.Value()
        elsif ref = ruby_peers[object_id] && peer = ref.object
          # Object was allocated by JavaScript and it already has a Ruby peer.
          return peer
        else
          # Object was allocated by JavaScript, but no peer yet.
          peer = super

          ref = V8::Weak::Ref.new(peer)
          ruby_peers[object_id] = ref
          ruby_peers
          cleanup = self.class.cleanup_peer_mapping(ruby_peers, object_id, ref)
          ObjectSpace.define_finalizer peer, cleanup
          return peer
        end
      else
        super
      end
    end

    def v8_peers
      @v8_peers ||= {}
    end

    def ruby_peers
      @ruby_pears ||= {}
    end

    private

    def peer_content_key
      return @peer_content_key if @peer_content_key

      isolate = context.native.isolate
      key = V8::C::String::NewFromUtf8(isolate, "rr::content")
      @peer_content_key  = V8::C::Symbol::For(isolate, key)
    end

    def self.cleanup_peer_mapping(map, object_id, value)
      map.delete(object_id) if map[object_id] == value
    end
  end
end
