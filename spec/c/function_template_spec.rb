require 'c_spec_helper'

describe V8::C::FunctionTemplate do
  requires_v8_context
  let(:function) { template.GetFunction(@ctx).FromJust() }

  describe "New()" do
    describe "with no arguments" do
      let(:template) { V8::C::FunctionTemplate::New(@isolate) }
      it "creates an empty template" do
        expect(template).to be
      end
      it "has an empty function" do
        expect(function).to be
        expect(function.Call(@ctx.Global(), []).StrictEquals(@ctx.Global())).to be true
      end
    end
    describe "with a function callback" do
      let(:data) { V8::C::Integer::New(@isolate, 42) }
      let(:signature) { V8::C::Signature::New(@isolate) }
      let(:template) { V8::C::FunctionTemplate::New(@isolate, @callback, data, @signature, 0) }
      before do
        @callback = ->(info) do
          @data = info.Data()
        end
        function.Call(@ctx.Global(), [])
      end

      it "does something" do
        expect(@data).not_to be_nil
        expect(@data.Value()).to be 42
      end
    end
  end
end
