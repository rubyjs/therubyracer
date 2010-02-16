require 'mkmf'

UPSTREAM = File.expand_path(File.dirname(__FILE__) + "/upstream")
BUILD = "#{UPSTREAM}/build/v8"


puts "Compiling V8"

system("cd #{UPSTREAM} && make") or raise "Error compiling V8"

dir_config('v8', "#{BUILD}/include", "#{BUILD}")
have_library('v8') or raise "Unable to find libv8 in #{BUILD}, was there an error compiling it?"

$CPPFLAGS += " -Wall" unless $CPPFLAGS.split.include? "-Wall"

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
