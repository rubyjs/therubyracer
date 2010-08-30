require 'set'
module V8
  class Access
    def get(obj, name, &dontintercept)
      methods = accessible_methods(obj)
      if methods.include?(name)
        method = obj.method(name)
        method.arity == 0 ? method.call : method.unbind
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
end 