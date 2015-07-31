require 'v8/weak'

class V8::Conversion
  ##
  # This is the fundamental conversion for from Ruby objects into
  # `V8::C::Value`s and vice-versa. For instances of `V8::C::Object`,
  # the result is stored in an identity map, so that subsequent
  # conversions return the same object both to and from ruby.
  #
  # It sits at the top of the conversion "middleware stack"
  module Fundamental
    ##
    # Convert a low-level `V8::C::Value` into a Ruby object,
    # optionally storing the result in an identity map so that it can
    # be re-used for subsequent conversions.
    #
    # @param [V8::C::Value] low-level C++ object.
    # @return [Object] Ruby object counterpart
    def to_ruby(v8_object)
      # Only objects can be reliably identified. If this is an
      # object, then we want to see if there is already a ruby object
      # associated with it.
      if v8_object.kind_of? V8::C::Object
        if rb_object = v8_idmap[v8_object.GetIdentityHash()]
          # it was in the id map, so return the existing instance.
          rb_object
        else
          # it was not in the id map, so we run the default conversion
          # and store it in the id map
          v8_object.to_ruby.tap do |object|
            equate object, v8_object
          end
        end
      else
        # not an object, just do the default conversion
        v8_object.to_ruby
      end
    end

    ##
    # Convert a Ruby object into a low-level C++ `V8::C::Value`.
    #
    # First it checks to see if there is an entry in the id map for
    # this object. Otherwise, it will run the default conversion.
    def to_v8(context, ruby_object)
      rb_idmap[ruby_object.object_id] || ruby_object.to_v8(context)
    end

    ##
    # Mark a ruby object and a low-level V8 C++ object as being
    # equivalent in this context.
    #
    # After being equated the two objects are like mirrors of each
    # other, where one exists in the Ruby world, and the other exists
    # in the V8 world. It's all very through the looking glass.
    #
    # Whenever `ruby_object` is reflected into the V8 runtime, then
    # `v8_object` will be used in its stead, and whenever `v8_object`
    # is reflected into the Ruby runtime, then `ruby_object` will be
    # used in *its* stead.
    #
    # @param [Object] the ruby object
    # @param [V8::C::Value] the v8 object
    def equate(ruby_object, v8_object)
      v8_idmap[v8_object.GetIdentityHash()] = ruby_object
      rb_idmap[ruby_object.object_id] = v8_object
    end

    def v8_idmap
      @v8_idmap ||= V8::Weak::WeakValueMap.new
    end

    def rb_idmap
      @ruby_idmap ||= V8::Weak::WeakValueMap.new
    end
  end
end
