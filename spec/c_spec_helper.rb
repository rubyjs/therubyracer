require 'v8/weak'
require 'v8/init'

module V8ContextHelpers
  module GroupMethods
    def requires_v8_context
      around(:each) do |example|
        bootstrap_v8_context(&example)
      end
    end
  end

  def bootstrap_v8_context
    isolate = V8::C::Isolate.New

    begin
      cleanup_isolates
      isolate.Enter

      # V8::C::Locker() do
        V8::C::HandleScope(isolate) do
          @cxt = V8::C::Context::New(isolate)
          begin
            @cxt.Enter
            yield
          ensure
            @cxt.Exit
          end
        end
      # end
    ensure
      isolate.Exit
    end
  end

  def cleanup_isolates
    V8::C::Isolate.GetCurrent.Exit while V8::C::Isolate.GetCurrent
  end
end

RSpec.configure do |c|
  c.include V8ContextHelpers
  c.extend V8ContextHelpers::GroupMethods
end
