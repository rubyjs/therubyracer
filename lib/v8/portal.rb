
module V8
  class Portal
    attr_reader :context, :proxies

    def js_constructor_for(ruby_class)
      unless template = @templates[ruby_class]
        template = @templates[ruby_class] = make_js_constructor(ruby_class)
      end
      return template
    end

    def invoke_non_callable_constructor(arguments)
      unless arguments.Length() == 1 && arguments[0].kind_of?(C::External)
        C::ThrowException(C::Exception::Error(C::String::New("cannot call native constructor from javascript")))
      else
        object = arguments[0].Value()
        @proxies.register_javascript_proxy arguments.This(), :for => object
      end
    end

    def invoke_callable_constructor(arguments)
      instance = nil
      if arguments.Length() > 0 && arguments[0].kind_of?(C::External)
        instance = arguments[0].Value()
      else
        rbargs = []
        for i in 0..arguments.Length() - 1
          rbargs << rb(arguments[i])
        end
        cls = arguments.Data()
        instance = rubysend(cls, :new, *rbargs)
      end
      @proxies.register_javascript_proxy arguments.This(), :for => instance
    rescue StandardError => e
      warn e
    end

    def make_js_constructor(cls)
      template = C::FunctionTemplate::New(method(:invoke_non_callable_constructor))
      setuptemplate(template.InstanceTemplate())
      if cls != ::Object && cls.superclass != ::Object && cls.superclass != ::Class
        template.Inherit(js_constructor_for(cls.superclass))
      end
      if cls.name && cls.name =~ /(::)?(\w+?)$/
        template.SetClassName(C::String::NewSymbol("rb::" + $2))
      else
        template.SetClassName("Ruby")
      end
      return template
    end

    def callable_js_constructor_for(ruby_class)
      @proxies.rb2js(ruby_class) do 
        constructor = js_constructor_for(ruby_class)
        function = constructor.GetFunction()
        unless constructor.respond_to?(:embedded)
          constructor.SetCallHandler(method(:invoke_callable_constructor), ruby_class)
          #create a prototype so that this constructor also acts like a ruby object
          prototype = rubytemplate.NewInstance()
          #set *that* object's prototype to an empty function so that it will look and behave like a function.
          prototype.SetPrototype(C::FunctionTemplate::New().GetFunction())
          function.SetPrototype(prototype)
          def constructor.embedded?;true;end
        end
        function
      end
    end

    def js_instance_for(ruby_object)
      constructor = js_constructor_for(ruby_object.class)
      arguments = C::Array::New(1)
      arguments.Set(0, C::External::New(ruby_object))
      constructor.GetFunction().NewInstance(arguments)
    end

    def initialize(context, access)
      @context, @access = context, access
      @proxies = Proxies.new
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

      @functions = Functions.new(self)
      
      #TODO: This is a memory leak!!
      @templates = {}
    end

    def open
      @context.native.enter do
        yield(self)
      end if block_given?
    end

    def rb(value)
      @proxies.js2rb(value) do
        case value
          when V8::C::Function    then V8::Function.new(value, self)
          when V8::C::Array       then V8::Array.new(value, self)
          when V8::C::Object      then V8::Object.new(value, self)
          when V8::C::String      then value.Utf8Value.tap {|s| return s.respond_to?(:force_encoding) ? s.force_encoding("UTF-8") : s}
          when V8::C::Date        then Time.at(value.NumberValue() / 1000)
          when V8::C::StackTrace  then V8::StackTrace.new(self, value)
          when V8::C::Value       then nil if value.IsEmpty()
        else
          value
        end
      end
    end

    def v8(value)
      case value
      when V8::Object
        value.instance_eval {@native}
      when String
        C::String::New(value)
      when Symbol
        C::String::NewSymbol(value.to_s)
      when Proc,Method,UnboundMethod
        @functions[value]
      when ::Array
        C::Array::New(value.length).tap do |a|
          value.each_with_index do |item, i|
            a.Set(i, v8(item))
          end
        end
      when ::Hash
        C::Object::New().tap do |o|
          value.each do |key, val|
            o.Set(v8(key), v8(val))
          end
        end
      when ::Time
        C::Date::New(value.to_f * 1000)
      when ::Class
        callable_js_constructor_for(value)
      when nil,Numeric,TrueClass,FalseClass, C::Value
        value
      else
        @proxies.rb2js(value, &method(:js_instance_for))
      end
    end

    def rubyprotect
      begin
        v8 yield
      rescue Exception => e
        case e
        when SystemExit, NoMemoryError
          raise e
        else
          error = V8::C::Exception::Error(V8::C::String::New(e.message))
          error.SetHiddenValue("TheRubyRacer::Cause", C::External::New(e))
          V8::C::ThrowException(error)
        end
      end
    end

    def rubycall(rubycode, *args, &block)
      rubyprotect do
        rubycode.call(*args, &block)
      end
    end

    def rubysend(obj, message, *args, &block)
      rubyprotect do
        obj.send(message, *args, &block)
      end
    end
    
    def rubytemplate
      C::ObjectTemplate::New().tap do |t|
        setuptemplate(t)
      end
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

    private

    class Interceptor
      def initialize(portal, access)
        @to, @access = portal, access
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
      cls.new self, @access
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
  end
end