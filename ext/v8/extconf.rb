require 'mkmf'
begin
  require 'libv8'
rescue LoadError
  require 'rubygems'
  require 'libv8'
end
include Libv8

have_library('objc') if RUBY_PLATFORM =~ /darwin/

#we have to manually prepend the libv8 include path to INCFLAGS
#since find_header() does not actually work as advertized.
#see https://github.com/cowboyd/therubyracer/issues/91
$INCFLAGS.insert 0, "#{libv8_include_flags} "

$CPPFLAGS += " -Wall" unless $CPPFLAGS.split.include? "-Wall"
$CPPFLAGS += " -g" unless $CPPFLAGS.split.include? "-g"
$CPPFLAGS += " -rdynamic" unless $CPPFLAGS.split.include? "-rdynamic"
$CPPFLAGS += " -fPIC" unless $CPPFLAGS.split.include? "-rdynamic" or RUBY_PLATFORM =~ /darwin/

$LDFLAGS.insert 0, libv8_objects.join(' ') + " "
$LIBS << ' -lpthread '


CONFIG['LDSHARED'] = '$(CXX) -shared' unless RUBY_PLATFORM =~ /darwin/

create_makefile('init')
