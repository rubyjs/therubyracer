require 'mkmf'

CONFIG['LDSHARED'] = "g++ -shared"

#dir_config('v8')
#have_library('v8')

create_makefile('v8')
