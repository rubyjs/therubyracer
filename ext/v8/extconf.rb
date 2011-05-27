require 'mkmf'
require 'set'
require 'libv8'

puts "Compiling The Ruby Racer..."

find_header('v8.h', Libv8.include_path)
have_library('pthread')
have_library('objc') if RUBY_PLATFORM =~ /darwin/

$CPPFLAGS += " -Wall" unless $CPPFLAGS.split.include? "-Wall"
$CPPFLAGS += " -g" unless $CPPFLAGS.split.include? "-g"
$CPPFLAGS += " -rdynamic" unless $CPPFLAGS.split.include? "-rdynamic"

$DEFLIBPATH.unshift(Libv8.library_path)
$LIBS << ' -lv8'

CONFIG['LDSHARED'] = '$(CXX) -shared' unless RUBY_PLATFORM =~ /darwin/

create_makefile('v8')
