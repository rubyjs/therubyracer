module V8
  class Portal
    class Proxies

      def initialize
        @js_proxies_rb2js = {}
        @js_proxies_js2rb = {}
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

      def clear_js_proxy(proxy)
        lambda do
          rb = @js_proxies_js2rb[proxy]
          @js_proxies_js2rb.delete(proxy)
          @js_proxies_rb2js.delete(rb)
        end
      end
    end
  end
end