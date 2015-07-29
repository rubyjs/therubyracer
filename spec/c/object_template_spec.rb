require 'c_spec_helper'

describe V8::C::ObjectTemplate do
  requires_v8_context
  let(:template) { V8::C::ObjectTemplate::New(@isolate) }

  it "can be used to create object instances" do
    expect(template.NewInstance(@ctx).FromJust()).to be_instance_of V8::C::Object
  end
end
