require 'mkmf'
require File.expand_path '../build', __FILE__

have_library('winmm') if V8::mingw?
have_library('pthread')
have_library('objc') if V8::darwin?
$CPPFLAGS += " -Wall" unless $CPPFLAGS.split.include? "-Wall"
$CPPFLAGS += " -g" unless $CPPFLAGS.split.include? "-g"
$CPPFLAGS += " -rdynamic" unless $CPPFLAGS.split.include? "-rdynamic" or V8::mingw?
$CPPFLAGS += " -fPIC" unless $CPPFLAGS.split.include? "-rdynamic" or V8::darwin? or V8::mingw?

CONFIG['LDSHARED'] = '$(CXX) -shared' unless V8::darwin? or V8::mingw?
if CONFIG['warnflags']
  CONFIG['warnflags'].gsub!('-Wdeclaration-after-statement', '')
  CONFIG['warnflags'].gsub!('-Wimplicit-function-declaration', '')
end
if enable_config('debug')
  $CFLAGS += " -O0 -ggdb3"
end
if have_rubygem_libv8?
  build_with_rubygem_libv8
else
  build_with_system_libv8
end

create_makefile('v8/init')
