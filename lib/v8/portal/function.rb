module V8
  class Portal
    class Function

      attr_reader :template, :function

      def initialize(portal, code)
        @portal = portal
        @template = V8::C::FunctionTemplate::New(code.respond_to?(:call) ? Call.new(portal) : BindAndCall.new(portal), code)
      end

      def function
        @template.GetFunction()
      end
      
      class Call
        def initialize(portal)
          @portal = portal
        end

        def call(arguments)
          proc = arguments.Data()
          rbargs = []
          for i in 0..arguments.Length() - 1
            rbargs << @portal.rb(arguments[i])
          end
          @portal.caller.invoke(proc, *rbargs)
        end
      end
      
      class BindAndCall < Call
        def call(arguments)
          method = arguments.Data()
          rbargs = []
          for i in 0..arguments.Length() - 1
            rbargs << @portal.rb(arguments[i])
          end
          this = @portal.rb(arguments.This())
          @portal.caller.protect do
            method.bind(this).call(*rbargs)
          end
        end
      end
    end
  end
end