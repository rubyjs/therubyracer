require 'c_spec_helper'

describe V8::C::External do
  let(:isolate) { V8::C::Isolate::New() }
  let(:value) { @external::Value() }
  around { |example| V8::C::HandleScope(isolate) { example.run } }

  before do
    Object.new.tap do |object|
      @object_id = object.object_id
      @external = V8::C::External::New(isolate, object)
    end
  end

  it "exists" do
    expect(@external).to be
  end

  it "can retrieve the ruby object out from V8 land" do
    expect(value).to be
    expect(value.object_id).to eql @object_id
  end
end
