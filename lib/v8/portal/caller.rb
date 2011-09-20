
module V8
  class Portal
    class Caller
      
      def initialize(portal)
        @portal = portal
      end
      
      def raw
        yield
      rescue Exception => e
        case e
        when SystemExit, NoMemoryError
          raise e
        else
          error = V8::C::Exception::Error(V8::C::String::New(e.message))
          #TODO: This is almost certainly a crash here.
          #we need to hold onto `error` while it bubbles up the javascript stack.
          error.SetHiddenValue("TheRubyRacer::Cause", C::External::New(e))
          V8::C::ThrowException(error)
        end
      end

      def protect(*args, &block)
        @portal.v8 raw(*args, &block)
      end

      def invoke(code, *args, &block)
        protect do
          code.call(*args, &block)
        end
      end
    end
  end
end