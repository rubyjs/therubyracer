require 'c_spec_helper'

describe V8::C::TryCatch do
  requires_v8_context

  before do
    @isolate.SetCaptureStackTraceForUncaughtExceptions(true, 99, V8::C::StackTrace::kDetailed)
  end

  it "can catch javascript exceptions" do
    V8::C::TryCatch(@isolate) do |trycatch|
      source = V8::C::String::NewFromUtf8(@isolate, <<-JS)
      function one() {
        two()
      }
      function two() {
        three()
      }
      function three() {
        boom()
      }
      function boom() {
        throw new Error('boom!')
      }
      eval('one()')
      JS


      filename = V8::C::String::NewFromUtf8(@isolate, "<eval>")
      origin = V8::C::ScriptOrigin.new(filename)
      script = V8::C::Script::Compile(@ctx, source, origin)
      expect(script).to be_successful
      result = script.FromJust().Run(@ctx)
      trycatch.HasCaught().should be_truthy
      trycatch.CanContinue().should be_truthy
      exception = trycatch.Exception()
      exception.should_not be_nil
      exception.IsNativeError().should be_truthy
      expect(trycatch.StackTrace(@ctx)).to be_successful
      trace = trycatch.StackTrace(@ctx).FromJust()
      trace.Utf8Value().should match /boom.*three.*two.*one/m
      message = trycatch.Message();
      message.should_not be_nil
      message.Get().Utf8Value().should eql "Uncaught Error: boom!"
      expect(message.GetSourceLine(@cxt)).to be_successful
      message.GetSourceLine(@cxt).FromJust().Utf8Value().should eql "        throw new Error('boom!')"
      message.GetScriptResourceName().Utf8Value().should eql "<eval>"
      expect(message.GetLineNumber(@cxt)).to be_successful
      expect(message.GetLineNumber(@cxt)).to eq_just 11

      stack = message.GetStackTrace()
      stack.should_not be_nil
      stack.GetFrameCount().should eql 6
      frame = stack.GetFrame(0)
      frame.GetLineNumber().should eql 11
      frame.GetColumn().should eql 15
      frame.GetScriptName().Utf8Value().should eql "<eval>"
      frame.IsEval().should be_falsey
      frame.IsConstructor().should be_falsey
    end
  end

  it "sees JavaScript exceptions thrown from Ruby" do
    V8::C::TryCatch(@isolate) do |trycatch|
      message = V8::C::String::NewFromUtf8(@isolate, "boom!")
      @isolate.ThrowException(V8::C::Exception::Error(message))
      expect(trycatch.HasCaught).to be_truthy
    end
  end

  it "won't die on a ruby exception" do
    expect {
      V8::C::TryCatch(@isolate) do |trycatch|
        fail "boom!"
      end
    }.to raise_error RuntimeError, "boom!"
  end
end
