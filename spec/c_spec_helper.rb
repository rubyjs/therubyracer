require 'v8/weak'
require 'v8/c/maybe'
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
    @isolate = V8::C::Isolate.New

    V8::C::HandleScope(@isolate) do
      @ctx = V8::C::Context::New(@isolate)
      begin
        @ctx.Enter
        yield
      ensure
        @ctx.Exit
      end
    end
  ensure
    @isolate.Dispose()
  end
end

RSpec.configure do |c|
  c.include V8ContextHelpers
  c.extend V8ContextHelpers::GroupMethods
end
