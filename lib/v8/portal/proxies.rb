module V8
  class Portal
    class Proxies

      def initialize
        @js_proxies_rb2js = {}
        @js_proxies_js2rb = {}
        @rb_proxies_rb2js = {}
        @rb_proxies_js2rb = {}
      end

      def js2rb(js)
        if rb = js_proxy_2_rb_object(js)
          return rb
        elsif rb = js_object_2_rb_proxy(js)
          return rb
        else
          proxy = block_given? ? yield(js) : Object.new
          register_ruby_proxy proxy, :for => js
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
          register_javascript_proxy proxy, :for => rb
          return proxy
        end
      end

      def register_javascript_proxy(proxy, options = {})
        target = options[:for] or fail ArgumentError, "must specify the object that you're proxying with the :for => param"
        fail ArgumentError, "javascript proxy must be a Handle to an actual V8 object" unless proxy.kind_of?(V8::C::Handle)

        @js_proxies_js2rb[proxy] = target
        @js_proxies_rb2js[target] = proxy
        proxy.MakeWeak(&clear_js_proxy(proxy))
      end

      def rb_object_2_js_proxy(object)
        @js_proxies_rb2js[object]
      end

      def js_proxy_2_rb_object(proxy)
        @js_proxies_js2rb[proxy]
      end

      def register_ruby_proxy(proxy, options = {})
        target = options[:for] or fail ArgumentError, "must specify the object that you're proxying with the :for => param"
        fail ArgumentError, "javascript proxy must be a Handle to an actual V8 object" unless target.kind_of?(V8::C::Handle)
        @rb_proxies_rb2js[proxy.object_id] = target
        @rb_proxies_js2rb[target] = proxy.object_id
        ObjectSpace.define_finalizer(proxy, method(:clear_rb_proxy))
      end

      def js_object_2_rb_proxy(object)
        if id = @rb_proxies_js2rb[object]
          ObjectSpace._id2ref id
        end
      end

      def rb_proxy_2_js_object(proxy)
        @rb_proxies_rb2js[proxy.object_id]
      end

      def clear_js_proxy(proxy)
        lambda do
          rb = @js_proxies_js2rb[proxy]
          @js_proxies_js2rb.delete(proxy)
          @js_proxies_rb2js.delete(rb)
        end
      end

      def clear_rb_proxy(proxy_id)
        js = @rb_proxies_rb2js[proxy_id]
        @rb_proxies_rb2js.delete(proxy_id)
        @rb_proxies_js2rb.delete(js)
      end
    end
  end
end