$:.unshift(File.dirname(__FILE__)) unless
  $:.include?(File.dirname(__FILE__)) || $:.include?(File.expand_path(File.dirname(__FILE__)))

module V8
  VERSION = '0.8.0'
  require 'v8/v8' #native glue
  require 'v8/portal'
  require 'v8/portal/functions'
  require 'v8/context'
  require 'v8/object'
  require 'v8/array'
  require 'v8/function'
  require 'v8/tap'
  require 'v8/access'
  require 'v8/error'
end