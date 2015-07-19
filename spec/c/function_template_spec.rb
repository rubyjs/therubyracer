require 'c_spec_helper'

describe V8::C::FunctionTemplate do
  requires_v8_context

  describe "New()" do
    describe "with no arguments" do
      let(:function) { template.GetFunction(@ctx).FromJust() }
      let(:template) { V8::C::FunctionTemplate::New(@isolate) }
      it "creates an empty template" do
        expect(template).to be
      end
      it "has an empty function" do
        expect(function).to be
        expect(function.Call(@ctx.Global(), []).StrictEquals(@ctx.Global())).to be true
      end
    end
  end
end
