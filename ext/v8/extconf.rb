require 'mkmf'
require 'set'
begin
  require 'libv8'
rescue LoadError
  require 'rubygems'
  require 'libv8'
end

have_library('objc') if RUBY_PLATFORM =~ /darwin/

#we have to manually prepend the libv8 include path to INCFLAGS
#since find_header() does not actually work as advertized.
#see https://github.com/cowboyd/therubyracer/issues/91
$INCFLAGS.insert 0, "-I#{Libv8.include_path} "

$CPPFLAGS += " -Wall" unless $CPPFLAGS.split.include? "-Wall"
$CPPFLAGS += " -g" unless $CPPFLAGS.split.include? "-g"
$CPPFLAGS += " -rdynamic" unless $CPPFLAGS.split.include? "-rdynamic"

$LDFLAGS.insert 0, "#{Libv8.library_path}/libv8.#{$LIBEXT} "
$LIBS << ' -lpthread'

CONFIG['LDSHARED'] = '$(CXX) -shared' unless RUBY_PLATFORM =~ /darwin/

create_makefile('v8')
