
begin
  require 'spec'
rescue LoadError
  require 'rubygems' unless ENV['NO_RUBYGEMS']
  gem 'rspec'
  require 'spec'
end

#
# $: is the load path $LOAD_PATH
#
$:.unshift(File.dirname(__FILE__) + '/../lib')
$:.unshift(File.dirname(__FILE__) + '/..')
require 'v8'

require 'erb'
def rputs(msg)
  puts "<pre>#{ERB::Util.h(msg)}</pre>"
  $stdout.flush
end
