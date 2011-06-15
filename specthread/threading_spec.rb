
require 'spec_helper'

describe "using v8 from multiple threads" do
  
  it "is possible" do
    Thread.new do
      require 'v8'
      V8::Context.new
    end.join
    V8::Context.new
  end
end