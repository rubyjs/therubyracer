
module V8
  module Jasmine
    FILENAME = File.join(File.dirname(__FILE__), "jasmine",Dir.new(File.join(File.dirname(__FILE__), "jasmine")).find {|f| f =~ /(\d+.\d+\.\d+)\.js$/})
    VERSION = $1
    SOURCE = File.read(FILENAME)
    
    class << self
      def included(mod)
        raise ScriptError, "#{self} cannot be included. Use cxt.extend(V8::Jasmine)"
      end
      
      def extended(cxt)
        raise ScriptError, "#{self} can only extend a V8::Context" unless cxt.kind_of?(V8::Context)  
        cxt.load(File.join(File.dirname(__FILE__), "jasmine", "window.js"))
        cxt.load(FILENAME)        
      end
    end
  end
end

require 'v8/jasmine/context'
