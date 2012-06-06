require 'v8'

def run_v8_gc
  while !V8::C::V8::IdleNotification() do
  end
end

def rputs(msg)
  puts "<pre>#{ERB::Util.h(msg)}</pre>"
  $stdout.flush
end

module ExplicitScoper;end
module Autoscope
  def instance_eval(*args, &block)
    return super unless low_level_c_spec? && !explicitly_defines_scope?
    V8::C::Locker() do
      V8::C::HandleScope() do
        @cxt = V8::C::Context::New()
        begin
          @cxt.Enter()
          super(*args, &block)
        ensure
          @cxt.Exit()
        end
      end
    end
  end

  def low_level_c_spec?
    name = described_class.name.split('::').last
    return V8::C.const_defined?(name) && V8::C.const_get(name) == described_class
  end

  def explicitly_defines_scope?
    is_a?(ExplicitScoper)
  end
end

RSpec.configure do |c|
  c.before(:each) do
    extend Autoscope
  end
end