class V8::Conversion
  module Identity
    def to_ruby(v8_object)
      v8_idmap.lookup(v8_object) {super}
    end

    def v8_idmap
      @v8_idmap ||= V8IDMap.new
    end

    class V8IDMap
      def initialize
        @storage = {}
      end

      def lookup(v8_object)
        if v8_object.is_a?(V8::C::Object)
          weakref = @storage[v8_object.GetIdentityHash()]
          if weakref && weakref.weakref_alive?
            weakref.__getobj__
          else
            @storage[v8_object.GetIdentityHash()] = WeakRef.new(yield)
          end
        else
          yield
        end
      end
    end
  end
end