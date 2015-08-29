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

  describe "SetCallHandler" do
    let(:data) { V8::C::Integer::New(@isolate, 42) }
    let(:template) { V8::C::FunctionTemplate::New(@isolate) }

    before do
      @callback = ->(info) do
        @data = info.Data()
      end
    end

    it "can set a callback" do
      template.SetCallHandler(@callback, data)
      function.Call(@ctx.Global(), [])

      expect(@data).to_not be_nil
      expect(@data.Value()).to be 42
    end
  end

  describe "InstanceTemplate" do
    let(:template) { V8::C::FunctionTemplate::New(@isolate) }

    it "returns an object template" do
      expect(template.InstanceTemplate).to be_a V8::C::ObjectTemplate
    end
  end

  describe "PrototypeTemplate" do
    let(:template) { V8::C::FunctionTemplate::New(@isolate) }

    it "returns an object template" do
      expect(template.PrototypeTemplate).to be_a V8::C::ObjectTemplate
    end
  end
end
