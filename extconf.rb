require 'mkmf'

dir_config('v8')
have_library('v8') or raise "unable to find libv8"

create_makefile('v8')
