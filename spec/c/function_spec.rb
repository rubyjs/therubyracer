require 'c_spec_helper'

describe V8::C::Function do
  requires_v8_context

  it "has a script origin" do
    fn = run '(function() { return "foo" })'
    origin = fn.GetScriptOrigin()
    expect(origin.ResourceName().ToString().Utf8Value()).to eql 'undefined'
    expect(origin.ResourceLineOffset().Value()).to eql 0
    expect(origin.ResourceColumnOffset().Value()).to eql 0
  end

  it 'can be called' do
    fn = run '(function() { return "foo" })'
    expect(fn.Call(@ctx.Global, []).Utf8Value).to eq 'foo'
  end

  it 'can be called with arguments and context' do
    fn = run '(function(one, two, three) {this.one = one; this.two = two; this.three = three})'

    one = V8::C::Object.New(@isolate)
    two = V8::C::Object.New(@isolate)

    fn.Call(@ctx.Global, [one, two, 3])

    expect(@ctx.Global.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'one'))).to strict_eq one
    expect(@ctx.Global.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'two'))).to strict_eq two
    expect(@ctx.Global.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'three')).FromJust().Value()).to eq 3
  end

  it 'can be called as a constructor' do
    fn = run '(function() {this.foo = "foo"})'
    expect(fn.NewInstance.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'foo')).FromJust().Utf8Value).to eq 'foo'
  end

  it 'can be called as a constructor with arguments' do
    fn = run '(function(foo) {this.foo = foo})'
    object = fn.NewInstance([V8::C::String.NewFromUtf8(@isolate, 'bar')])

    expect(object.Get(@ctx, V8::C::String.NewFromUtf8(@isolate, 'foo')).FromJust().Utf8Value).to eq 'bar'
  end

  describe "with a callback" do

    class FunctionCallback
      attr_reader :length, :callee, :this, :is_construct_call, :data
      def initialize(isolate)
        @isolate = isolate
      end
      def call(info)
        @length = info.Length()
        @callee = info.Callee()
        @this = info.This()
        @is_construct_call = info.IsConstructCall()
        @data = info.Data()

        ohai = V8::C::String::NewFromUtf8(@isolate, "ohai ")
        arg = info[0].ToString()
        result = V8::C::String::Concat(ohai, arg)
        info.GetReturnValue().Set(result)
      end
    end

    let(:data) { V8::C::Object::New(@isolate) }
    let(:callback) { FunctionCallback.new @isolate}
    let(:fn) { V8::C::Function::New(@isolate, callback, data)}

    before do
      expect(fn.Call(@ctx.Global(), ["world"]).Utf8Value()).to eql "ohai world"
    end


    it "has all of the function callback info available" do
      expect(callback.length).to eql 1
      expect(callback.callee.GetIdentityHash()).to eql fn.GetIdentityHash()
      expect(callback.this.GetIdentityHash()).to eql @ctx.Global().GetIdentityHash()
      expect(callback.is_construct_call).to be false
      expect(callback.data).not_to be_nil
      expect(callback.data).to strict_eq data
    end
  end


  # TODO
  # it 'doesn\'t kill the world if invoking it throws a javascript exception' do
  #   V8::C::TryCatch do
  #     fn = run '(function() { throw new Error("boom!")})'
  #     fn.Call(@ctx.Global(), [])
  #     fn.NewInstance([])
  #   end
  # end

  def run(source)
    source = V8::C::String.NewFromUtf8(@isolate, source.to_s)
    script = V8::C::Script.Compile(@ctx, source)
    result = script.FromJust().Run(@ctx)
    checked = result.FromJust()
    checked.kind_of?(V8::C::String) ? checked.Utf8Value : checked
  end
end
