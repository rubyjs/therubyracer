module V8
  class Portal
    class Functions
      def initialize(portal)
        @portal = portal
        @procs = {}
        @methods = {}
      end

      def to_function(code)
        case code
        when Method, UnboundMethod
          #TODO: clear this reference when the C::FunctionTemplate becomes weak.
          @methods[code.to_s] ||= Template.new(@portal, code.kind_of?(Method) ? :invoke_callable : :invoke_unbound_method, code)
        else
          if code.respond_to?(:call)
            #TODO: clear with reference when the C::FunctionTemplate becomes weak.
            @procs[code] ||= Template.new(@portal, :invoke_callable, code)
          else
            fail "invalid code type: #{code.class}"
          end
        end
      end

      class Template
        attr_reader :template, :function

        def initialize(portal, impl, code)
          @portal = portal
          @template = V8::C::FunctionTemplate::New(method(impl), code)
        end

        def function
          @template.GetFunction()
        end

        def invoke_callable(arguments)
          proc = arguments.Data()
          rbargs = []
          for i in 0..arguments.Length() - 1
            rbargs << @portal.rb(arguments[i])
          end
          @portal.rubycall(proc, *rbargs)
        end

        def invoke_unbound_method(arguments)
          method = arguments.Data()
          rbargs = []
          for i in 0..arguments.Length() - 1
            rbargs << @portal.rb(arguments[i])
          end
          this = @portal.rb(arguments.This())
          @portal.rubyprotect do
            method.bind(this).call(*rbargs)
          end
        end
      end
    end
  end
end