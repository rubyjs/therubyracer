module C
  module ContextHelper
    def self.included(base)
      base.instance_eval do
        before do
          @cxt = V8::C::Context::New()
          @cxt.Enter()
        end
        after do
          @cxt.Exit()
        end
      end
    end
  end
end