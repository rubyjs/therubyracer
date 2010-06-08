require 'set'
module V8
  
  #TODO each context should get its own access rules instead of sharing them across
  #     contetxts
  ###### --cowboyd 07/07/2010
  class Access
    def self.[](cls)
      @access ||= Access.new
      @access[cls]
    end

    def initialize
      @classes = {}
    end

    def [](cls)
      @classes ||= {}
      if ref = @classes[cls.object_id]
        if ref.weakref_alive?
          ref.__getobj__
        else
          @classes.delete(cls.object_id)
          self[cls]
        end
      else
        template(cls).tap do |t|
          t.InstanceTemplate().SetNamedPropertyHandler(
            NamedPropertyGetter,
            NamedPropertySetter,
            nil,
            nil,
            NamedPropertyEnumerator
          )
          if cls.name && cls.name =~ /(::)?(\w+?)$/
            t.SetClassName(C::String::NewSymbol("rb::" + $2))
          else
            t.SetClassName("Ruby")
          end
          @classes[cls.object_id] = WeakRef.new(t)
        end
      end
    end

    def template(cls)
      C::FunctionTemplate::New() do |arguments|
        unless arguments.Length() == 1 && arguments[0].kind_of?(C::External)
          C::ThrowException(C::Exception::Error(C::String::New("cannot call native constructor from javascript")))
        else
          arguments.This().tap do |this|
            this.SetHiddenValue(C::String::NewSymbol("TheRubyRacer::RubyObject"), arguments[0])              
          end
        end
      end
    end

    def self.rubyobject
      @rubyobject ||= C::ObjectTemplate::New().tap do |t|
        t.SetNamedPropertyHandler(
          NamedPropertyGetter,
          NamedPropertySetter,
          nil,
          nil,
        NamedPropertyEnumerator
        )
      end
    end
  end

  class Constructors < Access
    def self.[](cls)
      Access[cls].tap do |template|
        template.SetCallHandler() do |arguments|
          wrap = nil
          if arguments.Length() > 0 && arguments[0].kind_of?(C::External)
            wrap = arguments[0]
          else
            rbargs = []
            for i in 0..arguments.Length() - 1
              rbargs << To.rb(arguments[i])
            end
            instance = V8::Function.rubycall(cls.method(:new), *rbargs)
            wrap = C::External::New(instance)
          end
          arguments.This().tap do |this|
            this.SetHiddenValue(C::String::NewSymbol("TheRubyRacer::RubyObject"), wrap)
          end
        end
      end
    end
  end

  class NamedPropertyGetter
    def self.call(property, info)
      name = To.rb(property)
      obj = To.rb(info.This())
      perl_name = To.perl_case(name)
      methods = obj.public_methods(false).map {|m| m.to_s}
      method_name = if methods.include?(name)
        name
      elsif methods.include?(perl_name)
        perl_name
      end
      if method_name
        method = obj.method(method_name)
        if method.arity == 0
          Function.rubycall(method)
        else
          To.v8(method)
        end
      else
        C::Empty
      end
    end
  end

  class NamedPropertySetter
    def self.call(property, value, info)
      obj = To.rb(info.This())
      setter = To.rb(property) + "="
      camel_name = To.camel_case(setter)
      perl_name = To.perl_case(setter)
      methods = obj.public_methods(false).map {|m| m.to_s}
      if methods.include?(perl_name)
        obj.send(perl_name, To.rb(value))
      elsif methods.include?(camel_name)
        obj.send(camel_name, To.rb(value))
      else
         C::Empty
      end
    end
  end

  class NamedPropertyEnumerator
    def self.call(info)
      obj = To.rb(info.This())
      camel_methods = obj.public_methods(false).inject(Set.new) do |set, name|
        set.tap do
          set << To.camel_case(name)
        end
      end
      names = V8::C::Array::New(camel_methods.length)
      camel_methods.each_with_index do |name, i|
        names.Set(i, C::String::New(name))
      end
      return names
    end
  end
end