require 'c_spec_helper'

describe V8::C::ObjectTemplate do
  requires_v8_context
  let(:template) { V8::C::ObjectTemplate::New(@isolate) }

  it "can be used to create object instances" do
    expect(template.NewInstance(@ctx)).to be
  end
  it "can be used to create a new object" do

  end
end
