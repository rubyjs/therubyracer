require 'mkmf'

dir_config('v8')
have_library('v8') or raise "unable to find libv8"

create_makefile('v8')

# now add a few extra targets
File.open("Makefile", "a") do |makefile|
makefile.print <<EOF

test: all
	@echo running spec...
	spec -O spec/spec.opts spec/therubyracer_spec.rb
EOF
end
