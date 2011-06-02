module V8
  class Portal
    class Interceptors
      def initialize(portal)
        @getter = NamedPropertyGetter.new(portal)
        @setter = NamedPropertySetter.new(portal)
        @query = nil
        @deleter = nil
        @enumerator = NamedPropertyEnumerator.new(portal)
        @igetter = IndexedPropertyGetter.new(portal)
        @isetter = IndexedPropertySetter.new(portal)
        @iquery = nil
        @ideleter = nil
        @ienumerator = IndexedPropertyEnumerator.new(portal)
      end

      def setup(template)
        template.SetNamedPropertyHandler(@getter,@setter,@query,@deleter,@enumerator, nil)
        template.SetIndexedPropertyHandler(@igetter,@isetter,@iquery,@ideleter,@ienumerator, nil)
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

      class Interceptor

        def initialize(portal)
          @to, @access = portal, portal.access
        end

        def intercept(info, retval = nil, &code)
          obj = @to.rb(info.This())
          intercepts = true
          result = @to.caller.protect do
            dontintercept = proc do
              intercepts = false
            end
            code.call(obj, dontintercept)
          end
          intercepts ? (retval || result) : C::Empty
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
end

