require 'c_spec_helper'

describe V8::C::Function do
  requires_v8_context

  it "has a script origin" do
    fn = run '(function() { return "foo" })'
    origin = fn.GetScriptOrigin()
    expect(origin.ResourceName().ToString(@context).Utf8Value()).to eql 'undefined'
    expect(origin.ResourceLineOffset()).to eql 0
    expect(origin.ResourceColumnOffset()).to eql 0
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

    expect(@ctx.Global.Get(V8::C::String.NewFromUtf8(@isolate, 'one'))).to eq one
    expect(@ctx.Global.Get(V8::C::String.NewFromUtf8(@isolate, 'two'))).to eq two
    expect(@ctx.Global.Get(V8::C::String.NewFromUtf8(@isolate, 'three'))).to eq 3
  end

  it 'can be called as a constructor' do
    fn = run '(function() {this.foo = "foo"})'
    expect(fn.NewInstance.Get(V8::C::String.NewFromUtf8(@isolate, 'foo')).Utf8Value).to eq 'foo'
  end

  it 'can be called as a constructor with arguments' do
    fn = run '(function(foo) {this.foo = foo})'
    object = fn.NewInstance([V8::C::String.NewFromUtf8(@isolate, 'bar')])

    expect(object.Get(V8::C::String.NewFromUtf8(@isolate, 'foo')).Utf8Value).to eq 'bar'
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
    filename = V8::C::String.NewFromUtf8(@isolate, "<eval>")
    script = V8::C::Script.Compile(source, filename)
    result = script.Run(@ctx)

    result.kind_of?(V8::C::String) ? result.Utf8Value : result
  end
end
