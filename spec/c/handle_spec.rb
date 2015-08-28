require 'c_spec_helper'

describe V8::C::Handle do
  before do
    V8::C::V8::SetFlagsFromString("--expose_gc")
    @isolate = V8::C::Isolate::New()
    V8::C::HandleScope(@isolate) do
      @context = V8::C::Context::New(@isolate)
      @context.Enter()
      GC.stress = true
      2.times { v8_c_handle_spec_define_finalized_object(@isolate, self)}
      @context.Exit()
    end
    @isolate.RequestGarbageCollectionForTesting()
    @isolate.__EachV8Finalizer__ do |finalizer|
      finalizer.call
    end
  end
  after do
    GC.stress = false
    V8::C::V8::SetFlagsFromString("")
  end

  it "runs registered V8 finalizer procs when a v8 object is garbage collected" do
    expect(@did_finalize).to be >=  1
  end
end

def v8_c_handle_spec_did_finalize(spec)
  proc {
    spec.instance_eval do
      @did_finalize ||= 0
      @did_finalize += 1
    end
  }
end

def v8_c_handle_spec_define_finalized_object(isolate, spec)
  object = V8::C::Object::New(isolate)
  object.__DefineFinalizer__(v8_c_handle_spec_did_finalize(spec))
end
