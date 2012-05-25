require 'spec_helper'

describe "setting up handles scopes" do
  include C::ContextHelper
  it "can allocate handle scopes" do
    V8::C::HandleScope() do
      V8::C::Object::New()
    end.class.should eql V8::C::Object
  end

  it "isn't the end of the world if a ruby exception is raised inside a HandleScope" do
    begin
      V8::C::HandleScope() do
        raise "boom!"
      end
    rescue StandardError => e
      e.message.should eql "boom!"
    end
  end
end