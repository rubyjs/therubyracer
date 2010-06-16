require 'rbconfig'

def x86_64_from_build_cpu
  RbConfig::MAKEFILE_CONFIG['build_cpu'] == 'x86_64'
end

def x86_64_from_byte_length
  ['foo'].pack('p').size == 8 
end

def x86_64_from_arch_flag
  RbConfig::MAKEFILE_CONFIG['ARCH_FLAG'] =~ /x86_64/
end

def rubinius?
  Object.const_defined?(:RUBY_ENGINE) && RUBY_ENGINE == "rbx"
end

def x64?
  if rubinius?
    x86_64_from_build_cpu || x86_64_from_arch_flag
  else
    x86_64_from_byte_length
  end
end

puts x64? ? "x64" : "ia32"