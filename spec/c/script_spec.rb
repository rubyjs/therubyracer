require 'c_spec_helper'

describe V8::C::Script do
  requires_v8_context

  it 'can run a script and return a polymorphic result' do
    source = V8::C::String::NewFromUtf8(@isolate, "(new Array())")
    name = V8::C::String::NewFromUtf8(@isolate, "a/file.js")
    origin = V8::C::ScriptOrigin.new(name)
    script = V8::C::Script::Compile(@ctx, source, origin)
    expect(script.IsJust()).to be true

    result = script.FromJust().Run(@ctx)
    expect(result.IsJust()).to be true
    expect(result.FromJust()).to be_an V8::C::Array
  end

  # TODO
  # it 'can accept precompiled script data' do
  #   source = "7 * 6"
  #   name = V8::C::String::New("<spec>")
  #   origin = V8::C::ScriptOrigin.new(name)
  #   data = V8::C::ScriptData::PreCompile(source, source.length)
  #   data.HasError().should be_false
  #   script = V8::C::Script::New(V8::C::String::New(source), origin, data)
  #   script.Run().should eql 42
  # end

  # TODO
  # it 'can detect errors in the script data' do
  #   source = "^ = ;"
  #   data = V8::C::ScriptData::PreCompile(source, source.length)
  #   data.HasError().should be_true
  # end
end
