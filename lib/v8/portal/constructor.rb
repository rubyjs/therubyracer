
module V8
  class Portal
    class Constructor
      class Interceptor
        def initialize(portal)
          @to, @access = portal, portal.access
        end

        def intercept(info, retval = nil, &code)
          obj = @to.rb(info.This())
          intercepts = true
          result = @to.rubyprotect do
            dontintercept = proc do
              intercepts = false
            end
            code.call(obj, dontintercept)
          end
          intercepts ? (retval || result) : C::Empty
        end
      end

      def Interceptor(cls)
        cls.new portal
      end

      class PropertyAttributes
        attr_reader :flags
        def initialize
          @flags = 0
        end

        def read_only
          tap do
            @flags |= V8::C::ReadOnly
          end
        end

        def dont_enum
          tap do
            @flags |= V8::C::DontEnum
          end
        end

        def dont_delete
          tap do
            @flags |= V8::C::DontDelete
          end
        end
      end

      class NamedPropertyGetter < Interceptor
        def call(property, info)
          intercept(info) do |obj, dontintercept|
            @access.get(obj, @to.rb(property), &dontintercept)
          end
        end
      end

      class NamedPropertySetter < Interceptor
        def call(property, value, info)
          intercept(info, value) do |obj, dontintercept|
            @access.set(obj, @to.rb(property), @to.rb(value), &dontintercept)
          end
        end
      end

      class NamedPropertyQuery
        def call(property, info)
          attributes = PropertyAttributes.new
          result = intercept(info) do |obj, dontintercept|
            @access.query(obj, @to.rb(property), attributes, &dontintercept)
          end
          return result == C::Empty ? result : C::Integer::New(attributes.flags)
        end
      end

      class NamedPropertyEnumerator < Interceptor
        def call(info)
          intercept(info) do |obj, dontintercept|
            @access.names(obj, &dontintercept).to_a
          end
        end
      end

      class NamedPropertyDeleter < Interceptor
        def call(property, info)
          intercept(info) do |obj, dontintercept|
            @access.delete(obj, property, &dontintercept)
          end
        end
      end

      class IndexedPropertyGetter < Interceptor
        def call(index, info)
          intercept(info) do |obj, dontintercept|
            @access.iget(obj, index, &dontintercept)
          end
        end
      end

      class IndexedPropertySetter < Interceptor
        def call(index, value, info)
          intercept(info, value) do |obj, dontintercept|
            @access.iset(obj, index, @to.rb(value), &dontintercept)
          end
        end
      end

      class IndexedPropertyQuery < Interceptor
        def call(property, info)
          attributes = PropertyAttributes.new
          result = intercept(info) do |obj, dontintercept|
            @access.indices(obj, &dontintercept)
          end
          result == C::Empty ? C::Empty : C::Integer::New(attributes.flags)
        end
      end

      class IndexedPropertyDeleter < Interceptor
        def call(index, info)
          intercept(info) do |obj, dontintercept|
            @access.idelete(obj, index, &dontintercept)
          end
        end
      end

      class IndexedPropertyEnumerator < Interceptor
        def call(info)
          intercept(info) do |obj, dontintercept|
            @access.indices(obj, &dontintercept)
          end
        end
      end

      attr_reader :template, :function, :exposed
      alias_method :exposed?, :exposed

      def initialize(templates, class_id)
        @exposed = false
        @class_id = class_id
        @templates = templates

        @named_property_getter = Interceptor(NamedPropertyGetter)
        @named_property_setter = Interceptor(NamedPropertySetter)
        @named_property_query = nil
        @named_property_deleter = nil
        @named_property_enumerator = Interceptor(NamedPropertyEnumerator)

        @indexed_property_getter = Interceptor(IndexedPropertyGetter)
        @indexed_property_setter = Interceptor(IndexedPropertySetter)
        @indexed_property_query = nil
        @indexed_property_deleter = nil
        @indexed_property_enumerator = Interceptor(IndexedPropertyEnumerator)
    
        @template = C::FunctionTemplate::New(method(:invoke_unexposed))
        setuptemplate(@template.InstanceTemplate())
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
          setuptemplate(prototype_template)
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

      def setuptemplate(t)
        t.SetNamedPropertyHandler(
          @named_property_getter,
          @named_property_setter,
          nil,
          nil,
          @named_property_enumerator
        )
        t.SetIndexedPropertyHandler(
          @indexed_property_getter,
          @indexed_property_setter,
          nil,
          nil,
          @indexed_property_enumerator
        )
      end
    end
  end
end