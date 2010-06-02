require File.dirname(__FILE__) + '/../../spec_helper'

require 'v8/jasmine'

describe V8::Jasmine do

  it "cannot be included" do
    lambda {
      Class.new.send(:include, V8::Jasmine)
    }.should raise_error(ScriptError)
  end
  
  it "can only be used to extend V8::Context objecs" do
    lambda {
      V8::Context.new.extend(V8::Jasmine)
    }.should_not raise_error
    
    lambda {
      Object.new.extend(V8::Jasmine)
    }.should raise_error(ScriptError)
  end
  
  it "extends a bare context with the jasmine runtime" do
    V8::Context.new do |cxt|
      cxt.extend V8::Jasmine
      cxt['jasmine'].should_not be_nil
    end
  end
end

describe V8::Jasmine::Context do
  
  it "comes pre-bundled with jasmine" do
    V8::Jasmine::Context.new do |cxt|
      cxt['jasmine'].should_not be_nil
    end
  end
end