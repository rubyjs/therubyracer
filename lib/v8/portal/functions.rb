module V8
  class Portal
    class Functions
      def initialize(portal)
        @portal = portal
        @procs, @methods = {},{}
      end
      
      def [](code)
        self.send(code.class.name, code)
      end

      def Proc(p)
        @procs[p] ||= begin
          template = C::FunctionTemplate::New() do |arguments|
            rbargs = []
            for i in 0..arguments.Length() - 1
              rbargs << @portal.rb(arguments[i])
            end
            @portal.rubycall(p, *rbargs)
          end
          template.GetFunction()
        end
      end

      def UnboundMethod(method)
        @methods[method.to_s] ||= begin
          template = C::FunctionTemplate::New() do |arguments|
            rbargs = []
            for i in 0..arguments.Length() - 1
              rbargs << @portal.rb(arguments[i])
            end
            this = @portal.rb(arguments.This())
            @portal.rubyprotect do
              method.bind(this).call(*rbargs)
            end
          end
          template.GetFunction()
        end
      end

      alias_method :Method, :Proc
    end
  end
end