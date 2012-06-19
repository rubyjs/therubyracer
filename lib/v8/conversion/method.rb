class V8::Conversion
  module Method
    include V8::Conversion::Code

    def to_v8
      template = @@method_cache[self] ||= to_template
      template.GetFunction()
    end

    class MethodCache
      def initialize
        @map = {}
      end

      def [](method)
        weakref = @map[method.to_s]
        weakref.__getobj__ if weakref
      rescue WeakRef::RefError
        nil #template was garbage collected, so no mapping
      end

      def []=(method, template)
        @map[method.to_s] = WeakRef.new(template)
      end
    end

    @@method_cache = MethodCache.new
  end
end