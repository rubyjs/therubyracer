require 'v8'

def rputs(msg)
  puts "<pre>#{ERB::Util.h(msg)}</pre>"
  $stdout.flush
end

module V8ContextHelpers
  module GroupMethods
    def requires_v8_context
      around(:each) do |example|
        bootstrap_v8_context(&example)
      end
    end
  end

  def bootstrap_v8_context
    V8::C::Locker() do
      V8::C::HandleScope() do
        @cxt = V8::C::Context::New()
        begin
          @cxt.Enter()
          yield
        ensure
          @cxt.Exit()
        end
      end
    end
  end
end

RSpec.configure do |c|
  c.include V8ContextHelpers
  c.extend V8ContextHelpers::GroupMethods
end
