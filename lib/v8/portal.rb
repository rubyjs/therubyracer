
module V8
  class Portal
    attr_reader :context, :access, :proxies, :templates, :interceptors

    def initialize(context, access)
      @context, @access = context, access
      @proxies = Proxies.new
      @functions = Functions.new(self)
      @templates = Templates.new(self)
      @interceptors = Interceptors.new(self)
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
        @proxies.rb2js(value) do
          constructor = @templates.to_constructor(value)
          constructor.exposed = true
          constructor.function
        end
      when nil,Numeric,TrueClass,FalseClass, C::Value
        value
      else
        @proxies.rb2js(value) do
          @templates.to_constructor(value.class).allocate(value)
        end
      end
    end

    def rubyprotectraw
      begin
        yield
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

    def rubyprotect(*args, &block)
      v8 rubyprotectraw(*args, &block)
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

  end
end