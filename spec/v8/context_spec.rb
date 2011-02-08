require File.expand_path(File.dirname(__FILE__) + '/../spec_helper')
require 'ruby-debug'

class Window
  attr_accessor :document
end

describe V8::Context do

  #it "evaluates the same ruby object with different javascript variable names to be the same object" do   
    #context = V8::Context.new 
    #window = Window.new
    #context['window'] = window
    #context['top'] = window
    #context['another_window'] = Window.new
    #context['window'].should == context['top']
    #context['window'].should_not == context['another_window']
  #end

  # TODO Should there exist a V8::Hash class that contains a similar equals definition?
  # it "evaluates the same ruby hash with different javascript variable names to be the same object" do
  #   context = V8::Context.new 
  #   window = {} 
  #   context['window'] = window    
  #   context['top'] = window
  #   context['window'].should == context['top']
  # end

  it "evaluates the same ruby array with different javascript variable names to be the same object" do
    context = V8::Context.new 

    arr = [17,1]
    context['array'] = arr
    context['same_array'] = arr
    context['array'].should == context['same_array']
  end

  it "should make the same object equal" do
    V8::Context.new do |cxt|
      cxt.eval("var obj = {a: 'hi'}")
      cxt['obj'].should be {}
    end
  end

end
