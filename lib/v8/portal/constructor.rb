
module V8
  class Portal
    class Constructor
      attr_reader :template, :function, :exposed
      alias_method :exposed?, :exposed

      def initialize(templates, class_id)
        @exposed = false
        @class_id = class_id
        @templates = templates
    
        @template = C::FunctionTemplate::New(method(:invoke_unexposed))
        portal.interceptors.setup(@template.InstanceTemplate())
        cls = self.ruby_class
        superclass = cls.superclass
        if cls != ::Object && superclass != ::Object && superclass != ::Class
          @template.Inherit(templates.to_constructor(superclass).template)
        end
        if cls.name && cls.name =~ /(::)?(\w+?)$/
          template.SetClassName(C::String::NewSymbol("rb::" + $2))
        else
          template.SetClassName("Ruby")
        end
      end

      def function
        @template.GetFunction()
      end

      def allocate(object)
        arguments = C::Array::New(1)
        arguments.Set(0, C::External::New(object))
        function.NewInstance(arguments)
      end

      def invoke_unexposed(arguments)
        unless arguments.Length() == 1 && arguments[0].kind_of?(C::External)
          C::ThrowException(C::Exception::Error(C::String::New("cannot call native constructor from javascript")))
        else
          object = arguments[0].Value()
          proxies.register_javascript_proxy arguments.This(), :for => object
        end
      end

      def invoke_exposed(arguments)
        instance = nil
        if arguments.Length() > 0 && arguments[0].kind_of?(C::External)
          instance = arguments[0].Value()
        else
          rbargs = []
          for i in 0..arguments.Length() - 1
            rbargs << @templates.portal.rb(arguments[i])
          end
          instance = @templates.portal.rubyprotectraw do
            self.ruby_class.new(*rbargs)
          end
        end
        proxies.register_javascript_proxy arguments.This(), :for => instance
      rescue StandardError => e
        warn e
      end

      def exposed=(exposed)
        if exposed && !@exposed
          @template.SetCallHandler(method(:invoke_exposed))
          #create a prototype so that this constructor also acts like a ruby object
          prototype_template = C::ObjectTemplate::New()
          portal.interceptors.setup(prototype_template)
          prototype = prototype_template.NewInstance()
          #set *that* object's prototype to an empty function so that it will look and behave like a function.
          prototype.SetPrototype(C::FunctionTemplate::New().GetFunction())
          @function = @template.GetFunction()
          @function.SetPrototype(prototype)
          @exposed = true
        elsif !exposed && @exposed
          @template.SetCallHandler(method(:invoke_unexposed))
          @exposed = false
        else
          @exposed
        end
      end

      def ruby_class
        ObjectSpace._id2ref(@class_id)
      end

      def proxies
        @templates.proxies
      end

      def portal
        @templates.portal
      end
    end
  end
end