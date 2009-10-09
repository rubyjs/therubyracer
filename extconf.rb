require 'mkmf'

CONFIG['LDSHARED'] = "g++ -shared"

#CPP.sub!(CONFIG['CPP'], 'g++ -E')
#LINK.sub!(CONFIG['CC'], 'g++')


# dir_config('v8')
# have_library('v8')
create_makefile('v8')
