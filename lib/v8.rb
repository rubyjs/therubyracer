$:.unshift(File.dirname(__FILE__)) unless
  $:.include?(File.dirname(__FILE__)) || $:.include?(File.expand_path(File.dirname(__FILE__)))

module V8
  VERSION = '0.4.0'
  require 'v8/v8' #native glue
end