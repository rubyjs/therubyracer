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
        #TODO: check this for memory leaks
        @procs[p] ||= begin
          template = C::FunctionTemplate::New(method(:callproc), p)
          template.GetFunction()
        end
      end

      def UnboundMethod(method)
        #TODO: check this for memory leaks.
        @methods[method.to_s] ||= begin
          template = C::FunctionTemplate::New(method(:callmethod), method)
          template.GetFunction()
        end
      end

      alias_method :Method, :Proc

      def callproc(arguments)
        proc = arguments.Data()
        rbargs = []
        for i in 0..arguments.Length() - 1
          rbargs << @portal.rb(arguments[i])
        end
        @portal.rubycall(proc, *rbargs)
      end
      
      def callmethod(arguments)
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