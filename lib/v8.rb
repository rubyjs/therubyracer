$:.unshift(File.dirname(__FILE__)) unless
  $:.include?(File.dirname(__FILE__)) || $:.include?(File.expand_path(File.dirname(__FILE__)))

module V8
  require 'v8/version'
  require 'v8/v8' #native glue
  require 'v8/c/locker'
  require 'v8/portal'
  require 'v8/portal/caller'
  require 'v8/portal/proxies'
  require 'v8/portal/templates'
  require 'v8/portal/function'
  require 'v8/portal/constructor'
  require 'v8/portal/interceptors'
  require 'v8/context'
  require 'v8/object'
  require 'v8/array'
  require 'v8/function'
  require 'v8/tap'
  require 'v8/access'
  require 'v8/error'
  require 'v8/stack'
end