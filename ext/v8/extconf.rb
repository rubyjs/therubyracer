require 'mkmf'

puts "Compiling V8"

arch = ['foo'].pack('p').size == 8 ? 'x64' : 'ia32'

UPSTREAM_SRC = File.expand_path(File.dirname(__FILE__) + "/upstream")
SCONS_SRC = "#{UPSTREAM_SRC}/scons"
V8_SRC = "#{UPSTREAM_SRC}/2.0.6"
CCSC = "cd #{SCONS_SRC} && python setup.py install --prefix=#{SCONS_SRC}/install"
unless File.exists?("#{SCONS_SRC}/install")
puts CCSC
`#{CCSC}`
end
unless File.exists?("#{V8_SRC}/libv8.a")
CCV8 = "cd #{V8_SRC} && #{SCONS_SRC}/install/bin/scons arch=#{arch}"
puts CCV8
`#{CCV8}`
end

dir_config('v8', "#{V8_SRC}/include", "#{V8_SRC}")
have_library('v8')

$CPPFLAGS += " -Wall" unless $CPPFLAGS.split.include? "-Wall"

CONFIG['LDSHARED'] = '$(CXX) -shared'

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
