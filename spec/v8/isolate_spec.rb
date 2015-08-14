require 'spec_helper'

describe V8::Isolate do
  let(:isolate) { V8::Isolate.new }
  let(:context) { V8::Context.new isolate: isolate }

  it "allows you to terminate javascript execution" do
    context['doTerminate'] = lambda { isolate.terminate_execution! }
    expect(isolate).to_not be_execution_terminating
    expect {
      context.eval <<-JS
function doesTerminate() {
  doTerminate();
}
function returnFive() {
  return 5;
}
doesTerminate();
returnFive();
JS
    }.to raise_error V8::C::PendingExceptionError
  end

  it "allows you to cancel terminated javascript executing" do
    context['doTerminate'] = lambda do
      isolate.terminate_execution!
      isolate.cancel_terminate_execution!
    end
    result = context.eval <<-JS
function doesTerminate() {
  doTerminate();
}
function returnFive() {
  return 5;
}
doesTerminate();
returnFive();
JS
    expect(result).to eql 5
  end


end
