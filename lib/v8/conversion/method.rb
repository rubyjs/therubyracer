class V8::Conversion
  module Method
    include V8::Conversion::Proc

    def to_v8
      (@@method_cache[self] ||= v8_template).GetFunction()
    end


    class MethodCache
      def initialize
        @map = {}
      end

      def [](method)
        weakref = @map[method.to_s]
        if weakref && weakref.weakref_alive?
          weakref.__getobj__
        end
      end
      def []=(method, template)
        @map[method.to_s] = WeakRef.new(template)
      end
    end

    @@method_cache = MethodCache.new
  end
end