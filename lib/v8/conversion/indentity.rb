class V8::Conversion
  module Identity
    def to_ruby(v8_object)
      v8_idmap[v8_object] || super
    end

    def to_v8(ruby_object)
      rb_idmap[ruby_object] || super
    end

    def equate(ruby_object, v8_object)
      v8_idmap.equate(v8_object, ruby_object)
      rb_idmap.equate(ruby_object, v8_object)
    end

    def v8_idmap
      @v8_idmap ||= V8IDMap.new
    end

    def rb_idmap
      @ruby_idmap ||= RubyIDMap.new
    end

    class IDMap
      def initialize
        @map = {}
      end

      def [](object)
        weakref = @map[to_key(object)]
        if weakref && weakref.weakref_alive?
          weakref.__getobj__
        end
      end

      def equate(key_object, value_object)
        @map[to_key(key_object)] = WeakRef.new(value_object)
      end
    end

    class RubyIDMap < IDMap
      def to_key(object)
        object.object_id
      end
    end

    class V8IDMap < IDMap
      def to_key(object)
        object.GetIdentityHash()
      end

      def [](v8_object)
        super if v8_object.is_a?(V8::C::Object)
      end
    end
  end
end