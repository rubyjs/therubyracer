require 'v8'
load File.expand_path '../c/context_helper.rb', __FILE__

def run_v8_gc
  while !V8::C::V8::IdleNotification() do
  end
end

def rputs(msg)
  puts "<pre>#{ERB::Util.h(msg)}</pre>"
  $stdout.flush
end
