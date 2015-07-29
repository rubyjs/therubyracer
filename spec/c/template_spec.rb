require 'c_spec_helper'

describe V8::C::Template do
  requires_v8_context

  let(:key) { V8::C::String::NewFromUtf8(@isolate, "key") }
  let(:value) { V8::C::Integer::New(@isolate, 23) }
  let(:template) { V8::C::ObjectTemplate::New(@isolate) }

  describe "Set()" do

    describe "without property attributes" do
      before do
        template.Set(key, value)
      end
      it "places those values on all new instances" do
        object = template.NewInstance(@ctx).FromJust()
        expect(object.Get(@ctx, key).FromJust().Value).to be 23
      end
    end

    describe "with property attributes" do
      before do
        template.Set(key, value, V8::C::PropertyAttribute::ReadOnly)
        @object = template.NewInstance(@ctx).FromJust()
        @object.Set(@ctx, key, V8::C::Integer::New(@isolate, 25))
      end
      it "applies those property attributes" do
        expect(@object.Get(@ctx, key).FromJust().Value).to be 23
      end
    end
  end
end
