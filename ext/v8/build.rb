require File.expand_path('../../../lib/v8/helper', __FILE__)

LIBV8_COMPATIBILITY = '~> 3.11.8'

def have_rubygem_libv8?
  require 'rubygems'
  gem 'libv8', LIBV8_COMPATIBILITY
  require 'libv8'
  return true
rescue Gem::LoadError
  return false
rescue LoadError
  return false
end

def build_with_system_libv8
  if V8::mingw?
    v8_path = V8::v8_path
    if v8_path
      find_library('v8', nil, v8_path)
      find_header('v8.h', "#{v8_path}/include")
    end
  else
    dir_config('v8')
    $LDFLAGS << " -lv8 " #find_library('v8', nil)
    find_header('v8.h') 
  end or begin
    puts <<-EOS
    The Ruby Racer requires libv8 #{LIBV8_COMPATIBILITY}
    to be present on your system in order to compile
    and link, but it could not be found.

    In order to resolve this, you will either need to manually
    install an appropriate libv8 and make sure that this
    build process can find it. If you install it into the
    standard system path, then it should just be picked up
    automatically. Otherwise, you'll have to pass some extra
    flags to the build process as a hint.

    If you don't want to bother with all that, there is a
    rubygem that will do all this for you. You can add
    following line to your Gemfile:
        gem 'libv8', '#{LIBV8_COMPATIBILITY}'

    We hope that helps, and we apologize, but now we have
    to push the eject button on this install.

    thanks,
    The Mgmt.

    EOS
    raise "unable to locate libv8. Please see output for details"
  end

end

def build_with_rubygem_libv8
  require 'libv8'
  include Libv8
  $INCFLAGS.insert 0, "#{libv8_include_flags} "
  $LDFLAGS.insert 0, libv8_objects.join(' ') + " "
end
