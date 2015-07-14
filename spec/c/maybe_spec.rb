require 'c_spec_helper'

describe "maybe APIs" do
  requires_v8_context

  before do
    @isolate.ThrowException(V8::C::String::NewFromUtf8(@isolate, "boom!"))
  end

  let(:object) { object = V8::C::Object.New(@isolate) }

  it "throws an error when you want to " do
    object.ToString(@context)
  end

end
