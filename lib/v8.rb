$:.unshift(File.dirname(__FILE__)) unless
  $:.include?(File.dirname(__FILE__)) || $:.include?(File.expand_path(File.dirname(__FILE__)))

module V8
  VERSION = '0.6.0'
  require 'v8/v8' #native glue
  require 'v8/to'
  require 'v8/context'
  require 'v8/object'
  require 'v8/tap'
end