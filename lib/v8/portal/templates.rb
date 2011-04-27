
module V8
  class Portal
    class Templates

      attr_reader :portal

      def initialize(portal)
        @portal = portal
        @constructors = {}
        @methods = {}
        @procs = {}
      end
      
      def to_constructor(ruby_class)
        class_id = ruby_class.object_id
        if constructor = @constructors[class_id]
          return constructor
        else
          constructor = @constructors[class_id] = Constructor.new(self, class_id)
          ObjectSpace.define_finalizer(ruby_class, bind(@constructors, :delete, class_id))
          return constructor
        end
      end

      def to_function(code)
        case code
        when Method, UnboundMethod
          if fn = @methods[code.to_s]
            return fn
          else
            function = @methods[code.to_s] = Function.new(@portal, code)
            #TODO: test this weak behavior
            function.template.MakeWeak(0, bind(@methods, :delete, code.to_s))
            return function
          end
        else
          if fn = @procs[code]
            return fn
          else
            function = Function.new(@portal, code)
            #TODO: test this weak behavior
            function.template.MakeWeak(0, bind(@procs, :delete, code))
            return function
          end
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