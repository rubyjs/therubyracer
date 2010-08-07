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
          Access.setuptemplate(t.InstanceTemplate())
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
        setuptemplate(t)
      end
    end

    def self.setuptemplate(t)
      t.SetNamedPropertyHandler(
        NamedPropertyGetter,
        NamedPropertySetter,
        nil,
        nil,
        NamedPropertyEnumerator
      )
      t.SetIndexedPropertyHandler(
        IndexedPropertyGetter,
        IndexedPropertySetter,
        nil,
        nil,
        IndexedPropertyEnumerator
      )
    end
  end
  module AccessibleMethods
    def ruby
      @ruby ||= RubyAccess.new
    end
    def access(info, retval = nil, &code)
      obj = To.rb(info.This())
      intercepts = true
      result = Function.rubyprotect do
        dontintercept = proc do
          intercepts = false
        end
        code.call(obj, dontintercept)
      end
      intercepts ? (retval || result) : C::Empty
    end
  end

  class RubyAccess
    def get(obj, name, &dontintercept)
      methods = accessible_methods(obj)
      if methods.include?(name)
        method = obj.method(name)
        method.arity == 0 ? method.call : method
      elsif obj.respond_to?(:[])
        obj.send(:[], name, &dontintercept)
      else
        yield
      end
    end
    
    def iget(obj, index, &dontintercept)
      if obj.respond_to?(:[])
        obj.send(:[], index, &dontintercept)
      else
        yield
      end
    end

    def set(obj, name, value, &dontintercept)
      setter = name + "="
      methods = accessible_methods(obj)
      if methods.include?(setter)
        obj.send(setter, value)
      elsif obj.respond_to?(:[]=)
        obj.send(:[]=, name, value, &dontintercept)
      else
        yield
      end
    end

    def iset(obj, index, value, &dontintercept)
      if obj.respond_to?(:[]=)
        obj.send(:[]=, index, value, &dontintercept)
      else
        yield
      end
    end

    def accessible_methods(obj)
      obj.public_methods(false).map {|m| m.to_s}.to_set.tap do |methods|
        ancestors = obj.class.ancestors.dup
        while ancestor = ancestors.shift
          break if ancestor == ::Object
          methods.merge(ancestor.public_instance_methods(false).map {|m| m.to_s})
        end
        methods.reject! {|m| m == "[]" || m == "[]="}
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
            instance = V8::Function.rubysend(cls, :new, *rbargs)
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
    extend AccessibleMethods
    def self.call(property, info)
      access(info) do |obj, dontintercept|
        ruby.get(obj, To.rb(property), &dontintercept)
      end
    end
  end

  class NamedPropertySetter
    extend AccessibleMethods
    def self.call(property, value, info)
      access(info, value) do |obj, dontintercept|
        ruby.set(obj, To.rb(property), To.rb(value), &dontintercept)
      end
    end
  end

  class NamedPropertyQuery
    extend AccessibleMethods
    class Attrs
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

    def self.call(property, info)
      attrs = Attrs.new
      intercepts = true
      access_query(To.rb(info.This()), To.rb(property), attrs) do
        intercepts = false
      end
      intercepts ? C::Integer::New(attrs.flags) : C::Empty
    end

    def self.access_attributes(obj, name, attrs)
      unless obj.respond_to?(name)
        attrs.dont_enum
        attrs.dont_delete
      end
      unless obj.respond_to?("name=")
        attrs.read_only
      end
    end
  end

  class NamedPropertyEnumerator
    extend AccessibleMethods
    def self.call(info)
      obj = To.rb(info.This())
      methods = ruby.accessible_methods(obj)
      methods.reject! {|m| m.to_s =~ /=$/}
      names = V8::C::Array::New(methods.length)
      methods.each_with_index do |name, i|
        names.Set(i, C::String::New(name))
      end
      return names
    end
  end

  class IndexedPropertyGetter
    extend AccessibleMethods
    def self.call(index, info)
      access(info) do |obj, dontintercept|
        ruby.iget(obj, index, &dontintercept)
      end
    end
  end

  class IndexedPropertySetter
    extend AccessibleMethods
    def self.call(index, value, info)
      access(info, value) do |obj, dontintercept|
        ruby.iset(obj, index, To.rb(value), &dontintercept)
      end
    end
  end

  class IndexedPropertyEnumerator
    def self.call(info)
      obj = To.rb(info.This())
      if obj.respond_to?(:length)
        C::Array::New(obj.length).tap do |indices|
          for index in 0..obj.length - 1
            indices.Set(index,index)
          end
        end
      else
        C::Array::New()
      end
    end
  end
end 