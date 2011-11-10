require 'v8'

def rputs(msg)
  puts "<pre>#{ERB::Util.h(msg)}</pre>"
  $stdout.flush
end
