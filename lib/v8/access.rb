require 'set'
module V8
  
  class Access
    def initialize(portal)
      @classes = Hash.new do |h, cls|
        h[cls] = template(cls).tap do |t|
          portal.setuptemplate(t.InstanceTemplate())
          if cls.name && cls.name =~ /(::)?(\w+?)$/
            t.SetClassName(C::String::NewSymbol("rb::" + $2))
          else
            t.SetClassName("Ruby")
          end
        end
      end
      @impl = RubyAccess.new
    end

    def [](cls)
      @classes[cls]
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

    def method_missing(name, *args, &blk)
      @impl.send(name, *args, &blk)
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
      methods = accessible_methods(obj, true)
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

    def query(obj, name, attributes)
      if obj.respond_to?(name)
        attributes.dont_delete
        unless obj.respond_to?(name + "=")
          attributes.read_only
        end
      else
        yield
      end
    end

    def iquery(obj, index, attributes)
      if obj.respond_to?(:[])
        attributes.dont_delete
        unless obj.respond_to?(:[]=)
          attributes.read_only
        end
      else
        yield
      end
    end

    def names(obj)
      accessible_methods(obj)
    end

    def indices(obj)
      obj.respond_to?(:length) ? (0..obj.length).to_a : yield
    end

    private

    def accessible_methods(obj, special_methods = false)
      obj.public_methods(false).map {|m| m.to_s}.to_set.tap do |methods|
        ancestors = obj.class.ancestors.dup
        while ancestor = ancestors.shift
          break if ancestor == ::Object
          methods.merge(ancestor.public_instance_methods(false).map {|m| m.to_s})
        end
        methods.reject! {|m| m == "[]" || m == "[]=" || m =~ /=$/} unless special_methods
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
end 