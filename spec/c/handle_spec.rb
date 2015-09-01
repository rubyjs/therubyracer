require 'c_spec_helper'

describe V8::C::Handle do
  before { V8::C::V8::SetFlagsFromString("--expose_gc") }
  after { V8::C::V8::SetFlagsFromString("") }

  describe "Registering a v8 GC callback" do
    before do
      @isolate = V8::C::Isolate::New()
      V8::C::HandleScope(@isolate) do
        @context = V8::C::Context::New(@isolate)
        @context.Enter()
        @object = V8::C::Object::New(@isolate)
        @results = {}
        @object.SetWeak(v8_c_handle_spec_did_finalize(@results))
        @context.Exit()
      end
      @isolate.RequestGarbageCollectionForTesting()
      @isolate.__EachV8Finalizer__(&:call)
    end
    after do
      V8::C::V8::SetFlagsFromString("")
    end

    it "runs registered V8 finalizer procs when a v8 object is garbage collected" do
      expect(@results[:did_finalize]).to be_truthy
    end
  end

  describe "Capturing a new handle to an existing object" do
    before do
      @isolate = V8::C::Isolate::New()
      V8::C::HandleScope(@isolate) do
        @context = V8::C::Context::New(@isolate)
        @context.Enter()
        @object = V8::C::Object::New(@isolate)
        @other = V8::C::Handle::New(@object)
        @context.Exit()
      end
    end
    it "can call methods on it" do
      V8::C::HandleScope(@isolate) do
        expect(@other.GetIdentityHash()).to eq @object.GetIdentityHash()
      end
    end

    describe ". Then making the original handle weak" do
      before do
        V8::C::HandleScope(@isolate) do
          @results = {}
          @object.SetWeak(v8_c_handle_spec_did_finalize(@results))
        end
        @isolate.RequestGarbageCollectionForTesting()
        @isolate.__EachV8Finalizer__(&:call)
      end
      it "does not gc the v8 object" do
        V8::C::HandleScope(@isolate) do
          expect(@object.IsEmpty()).to_not be_truthy
          expect(@results[:did_finalize]).to_not be_truthy
        end
      end

      describe ". Then making the second handle weak" do
        before do
          V8::C::HandleScope(@isolate) do
            @other.SetWeak(v8_c_handle_spec_did_finalize(@results))
          end
          @isolate.RequestGarbageCollectionForTesting()
          @isolate.__EachV8Finalizer__(&:call)
        end
        it "garbage collects the v8 object" do
          expect(@results[:did_finalize]).to be_truthy
        end
        it "indicates that both object handles are now empty" do
          V8::C::HandleScope(@isolate) do
            expect(@object.IsEmpty()).to be_truthy
            expect(@other.IsEmpty()).to be_truthy
          end
        end
      end
    end
  end

  def v8_c_handle_spec_did_finalize(results)
    proc {
      results[:did_finalize] = true
    }
  end
end
