require File.dirname(__FILE__) + '/../spec_helper'

include V8

describe V8::To do
  
  it "can convert into perl case" do
    To.perl_case("foo").should == "foo"
    To.perl_case("fooBar").should == "foo_bar"
    To.perl_case("fooBarBaz").should == "foo_bar_baz"
    To.perl_case("XMLDocument").should == "xml_document"
  end
  
  
end