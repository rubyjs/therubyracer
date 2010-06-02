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

# now add a few extra targets
File.open("Makefile", "a") do |makefile|
makefile.print <<EOF

test: all
	@echo running spec...
	spec -O spec/spec.opts spec/therubyracer_spec.rb

docs/cpp:
	mkdir -p docs/cpp

docs: all docs/cpp
	@echo Generate C++ docs to docs/cpp/html
	doxygen Doxyfile

EOF
end
