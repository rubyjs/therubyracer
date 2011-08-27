
require 'spec_helper'

describe "using v8 from multiple threads" do
  
  it "creates contexts from within threads" do
    10.times.collect do
      Thread.new do
        require 'v8'
        V8::Context.new
      end
    end.each {|t| t.join}
    V8::Context.new
  end
  
  it "executes codes on multiple threads simultaneously" do
    5.times.collect{V8::Context.new}.collect do |ctx|
      Thread.new do
        ctx['x'] = 99
        while ctx['x'] > 0
          ctx.eval 'for (i=10000;i;i--){};--x'
        end
      end
    end.each {|t| t.join}
  end
  
end