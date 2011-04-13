
require 'v8'
require 'erb'
require Pathname(__FILE__).dirname.join('ext/ext_spec_helper')
def rputs(msg)
  puts "<pre>#{ERB::Util.h(msg)}</pre>"
  $stdout.flush
end
