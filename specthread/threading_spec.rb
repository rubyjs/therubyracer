
require 'spec_helper'

describe "using v8 from multiple threads" do
  
  it "is possible" do
    2.times.collect do
      Thread.new do
        require 'v8'
        V8::Context.new
      end
    end.each {|t| t.join}
    V8::Context.new
  end
end