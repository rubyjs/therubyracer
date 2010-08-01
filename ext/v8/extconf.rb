require 'mkmf'
require 'set'

UPSTREAM = File.expand_path(File.dirname(__FILE__) + "/upstream")
BUILD = "#{UPSTREAM}/build/v8"


puts "Compiling V8"

system("cd #{UPSTREAM} && make") or raise "Error compiling V8"

find_header('v8.h', "#{BUILD}/include")
have_library('pthread')
have_library('objc') if RUBY_PLATFORM =~ /darwin/

$CPPFLAGS += " -Wall" unless $CPPFLAGS.split.include? "-Wall"
$CPPFLAGS += " -g" unless $CPPFLAGS.split.include? "-g"
$CPPFLAGS += " -rdynamic" unless $CPPFLAGS.split.include? "-rdynamic"

$DEFLIBPATH.unshift(BUILD)
$LIBS << ' -lv8'

CONFIG['LDSHARED'] = '$(CXX) -shared' unless RUBY_PLATFORM =~ /darwin/

create_makefile('v8')
