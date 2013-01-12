require 'mkmf'

have_library('pthread')
have_library('objc') if RUBY_PLATFORM =~ /darwin/
$CPPFLAGS += " -Wall" unless $CPPFLAGS.split.include? "-Wall"
$CPPFLAGS += " -g" unless $CPPFLAGS.split.include? "-g"
$CPPFLAGS += " -rdynamic" unless $CPPFLAGS.split.include? "-rdynamic"
$CPPFLAGS += " -fPIC" unless $CPPFLAGS.split.include? "-rdynamic" or RUBY_PLATFORM =~ /darwin/

CONFIG['LDSHARED'] = '$(CXX) -shared' unless RUBY_PLATFORM =~ /darwin/
if CONFIG['warnflags']
  CONFIG['warnflags'].gsub!('-Wdeclaration-after-statement', '')
  CONFIG['warnflags'].gsub!('-Wimplicit-function-declaration', '')
end
if enable_config('debug')
  $CFLAGS += " -O0 -ggdb3"
end

require 'rubygems'
gem 'libv8', '~> 3.15.11'
require 'libv8'

Libv8.configure_makefile

create_makefile('v8/init')
