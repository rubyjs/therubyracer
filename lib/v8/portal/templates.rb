
module V8
  class Portal
    class Templates

      attr_reader :portal

      def initialize(portal)
        @portal = portal
        @constructors = {}
      end
      
      def to_constructor(ruby_class)
        class_id = ruby_class.object_id
        if constructor = @constructors[class_id]
          return constructor
        else
          constructor = @constructors[class_id] = Portal::Constructor.new(self, class_id)
          ObjectSpace.define_finalizer(ruby_class, bind(@constructors, :delete, class_id))
          return constructor
        end
      end

      def proxies
        @portal.proxies
      end

      def bind(object, method, *args)
        lambda {object.send(:method, *args)}
      end
    end
  end
end