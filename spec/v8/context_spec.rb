require File.expand_path(File.dirname(__FILE__) + '/../spec_helper')
require 'ruby-debug'

class Window
  attr_accessor :document
end

describe V8::Context do

  it "evaluates the same ruby object with different javascript variable names to be the same object" do   
    context = V8::Context.new 
    window = Window.new 
    context['window'] = window    
    context['top'] = window
    context['window'].should == context['top']
  end

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
    window = [7] 
    context['window'] = window    
    context['top'] = window
    context['window'].should == context['top']
  end

end