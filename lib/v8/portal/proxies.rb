module V8
  class Portal
    class Proxies

      def initialize
        @js_proxies = {}
        @rb_proxies = {}
      end

      def register_javascript_proxy(proxy, options = {})
        target = options[:for] or fail ArgumentError, "must specify the object that you're proxying with the :for => param"
        fail ArgumentError, "javascript proxy must be a Handle to an actual V8 object" unless proxy.kind_of?(V8::C::Handle)
        # handle = V8::C::Handle::New(proxy)
        # handle.MakeWeak(&clear_jsproy)
        @js_proxies[proxy.object_id] = target
      end
    end
  end
end