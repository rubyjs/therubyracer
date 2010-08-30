module V8
  class Portal
    class Functions
      def initialize(portal)
        @portal = portal
        @procs, @methods = {},{}
      end
      
      def [](code)
        case code
        when Proc
          @procs[code] ||= begin
            template = C::FunctionTemplate::New() do |arguments|
              rbargs = []
              for i in 0..arguments.Length() - 1
                rbargs << @portal.rb(arguments[i])
              end
              @portal.rubycall(code, *rbargs)
            end
            template.GetFunction()
          end
        when Method
          get_method(code)
        end
      end
      
      def get_method(method)
        unbound = method.unbind
        @methods[unbound.to_s] ||= begin
          template = C::FunctionTemplate::New() do |arguments|
            rbargs = []
            for i in 0..arguments.Length() - 1
              rbargs << @portal.rb(arguments[i])
            end
            this = @portal.rb(arguments.This())
            begin
              @portal.rubycall(unbound.bind(this), *rbargs)
            rescue Exception => e
              raise e
            end
          end
          template.GetFunction()
        end
      end
    end
  end
end