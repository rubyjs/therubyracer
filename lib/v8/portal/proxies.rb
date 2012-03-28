module V8
  class Portal
    class Proxies

      def initialize
        @js_proxies_rb2js = {}
        @js_proxies_js2rb = {}
        @rb_proxies_rb2js = {}
        @rb_proxies_js2rb = {}
        @clear_js_proxy = ClearJSProxy.new(@js_proxies_rb2js, @js_proxies_js2rb)
        @clear_rb_proxy = ClearRubyProxy.new(@rb_proxies_rb2js, @rb_proxies_js2rb)
      end

      def js2rb(js)
        if rb = js_proxy_2_rb_object(js)
          return rb
        elsif rb = js_object_2_rb_proxy(js)
          return rb
        else
          proxy = block_given? ? yield(js) : Object.new
          register_ruby_proxy proxy, :for => js if proxy && js && js.kind_of?(V8::C::Handle)
          return proxy
        end
      end

      def rb2js(rb)
        if js = rb_proxy_2_js_object(rb)
          return js
        elsif js = rb_object_2_js_proxy(rb)
          return js
        else
          proxy = block_given? ? yield(rb) : V8::C::Object::New()
          register_javascript_proxy proxy, :for => rb unless @js_proxies_rb2js[rb]
          return proxy
        end
      end

      def register_javascript_proxy(proxy, options = {})
        target = options[:for] or fail ArgumentError, "must specify the object that you're proxying with the :for => param"
        fail ArgumentError, "javascript proxy must be a V8::C::Handle, not #{proxy.class}" unless proxy.kind_of?(V8::C::Handle)
        fail DoubleProxyError, "target already has a registered proxy" if @js_proxies_rb2js[target]

        @js_proxies_js2rb[proxy] = target
        @js_proxies_rb2js[target] = proxy
        proxy.MakeWeak(nil, @clear_js_proxy)
      end

      # Lookup the JavaScript proxy for a natively Ruby object
      # @param [Object] object the Ruby object
      # @return [V8::C::Handle] the JavaScript proxy representing `object`
      def rb_object_2_js_proxy(object)
        @js_proxies_rb2js[object]
      end

      # Look up a natively Ruby object given its JavaScript proxy
      # @param [V8::C::Handle] proxy the JavaScript proxy
      # @return [Object] the Ruby object represented by `proxy`
      def js_proxy_2_rb_object(proxy)
        @js_proxies_js2rb[proxy]
      end

      def register_ruby_proxy(proxy, options = {})
        target = options[:for] or fail ArgumentError, "must specify the object that you're proxying with the :for => param"
        fail ArgumentError, "'#{proxy.inspect}' is not a Handle to an actual V8 object" unless target.kind_of?(V8::C::Handle)
        @rb_proxies_rb2js[proxy.object_id] = target
        @rb_proxies_js2rb[target] = proxy.object_id
        ObjectSpace.define_finalizer(proxy, @clear_rb_proxy)
      end

      # Looks up the Ruby proxy for an object that is natively JavaScript
      # @param [V8::C::Handle] object the JavaScript whose proxy we want
      # @return [Object] the Ruby proxy for `object`
      def js_object_2_rb_proxy(object)
        if id = @rb_proxies_js2rb[object]
          ObjectSpace._id2ref id
        end
      rescue RangeError
        # sometimes, the Ruby proxy has been garbage collected, but
        # the finalizer which runs has not been called. That's OK
        # we just clear out the entry, and return nil so that a new
        # proxy can be created.
        @clear_rb_proxy.call(id)
        return nil
      end

      # Looks up a native JavaScript object by its Ruby proxy
      # @param [Object] proxy the Ruby proxy
      # @return [V8::C::Handle] the native JavaScript object
      def rb_proxy_2_js_object(proxy)
        @rb_proxies_rb2js[proxy.object_id]
      end

      def js_empty?
        @js_proxies_rb2js.empty? && @js_proxies_js2rb.empty?
      end

      def rb_empty?
        @rb_proxies_rb2js.empty? && @rb_proxies_js2rb.empty?
      end

      def empty?
        js_empty? && rb_empty?
      end
      DoubleProxyError = Class.new(StandardError)

      class ClearJSProxy

        def initialize(rb2js, js2rb)
          @rb2js, @js2rb = rb2js, js2rb
        end

        def call(proxy, parameter)
          rb = @js2rb[proxy]
          @js2rb.delete(proxy)
          @rb2js.delete(rb)
        end
      end

      # @Private
      # Remove the linkage between a Ruby proxy and a native
      # JavaScript object. In general, this object is registered
      # as a finalizer on the Ruby proxy itself, so that when it is
      # garbage collected, it releases the back reference to the
      # native JavaScript object.
      #
      # It is important to do this as soon as is reasonably possible
      # so that the native JavaScript object can itself be garbage 
      # collected (provided there are no other references to it)
      class ClearRubyProxy
        def initialize(rb2js, js2rb)
          @rb2js, @js2rb = rb2js, js2rb
        end

        # takes the object id of a Ruby proxy that has been garbage collected
        # and releases the reference to the native JavaScript object that
        # it was bound to.
        # @param[Fixnum] proxy_id the proxy id of the garbage collected Ruby proxy
        def call(proxy_id)
          # TODO: this if-check should be synchronized, so that if called manually
          # it will not conflict with the finalization thread. It's not so heinous
          # if the refererence gets cleared twice, but we definiteily dont't want
          # to double-decrement the v8 GC hint.
          if js = @rb2js[proxy_id]
            @rb2js.delete(proxy_id)
            @js2rb.delete(js)
          end
        end
      end
    end
  end
end