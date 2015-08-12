require 'c_spec_helper'

describe V8::C::StackTrace do
  let(:const) { V8::C::StackTrace }
  it "defines the stack trace description constants" do
    expect(const::kLineNumber).to eql 1
    expect(const::kColumnOffset).to eql 3
    expect(const::kScriptName).to eql 4
    expect(const::kFunctionName).to eql 8
    expect(const::kIsEval).to eql 16
    expect(const::kIsConstructor).to eql 32
    expect(const::kScriptNameOrSourceURL).to eql 64
    expect(const::kScriptId).to eql 128
    expect(const::kExposeFramesAcrossSecurityOrigins).to eql 256
    expect(const::kOverview).to eql 15
    expect(const::kDetailed).to eql 127
  end
end
