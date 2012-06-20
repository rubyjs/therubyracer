class V8::Conversion
  module Method
    include V8::Conversion::Code

    def to_v8
      template = @@method_cache[self] ||= to_template
      template.GetFunction()
    end

    class MethodCache
      def initialize
        @map = Ref::WeakValueMap.new
      end

      def [](method)
        @map[method.to_s]
      end

      def []=(method, template)
        @map[method.to_s] = template
      end
    end

    @@method_cache = MethodCache.new
  end
end