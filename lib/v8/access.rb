require 'set'
module V8
  
  class NamedPropertyGetter
    def self.call(property, info)
      name = To.rb(property)
      obj = To.rb(info.This())
      perl_name = To.perl_case(name)
      methods = obj.public_methods(false).map(&:to_s)
      method_name = if methods.include?(name)
        name
      elsif methods.include?(perl_name)
        perl_name
      end
      if method_name
        method = obj.method(method_name)
        if method.arity == 0
          To.v8(method.call())
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
      methods = obj.public_methods(false).map(&:to_s)
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