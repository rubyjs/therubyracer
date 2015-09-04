require 'spec_helper'

describe "V8::Identity" do
  let(:cxt) { V8::Context.new }

  describe "when no strong reference remains to a Ruby object, and the garbage collector runs" do
    before do
      @ref = V8::Weak::Ref.new(Object.new)
      GC.start(full_mark: true, immediate_sweep: true)
    end

    it "gets garbage collected" do
      expect(@ref.object).to be_nil
    end
  end

  describe "adding proxying a Ruby object into v8" do
    before do
      @ref  = V8::Weak::Ref.new(cxt['one'] = Object.new)
      GC.start(full_mark: true, immediate_sweep: true)
    end

    it "returns the same object" do
      expect(@ref.object).to be cxt['one']
    end

    it "does not garbage collect the value embedded in the context" do
      expect(@ref.object).to_not be_nil
    end

    describe "removing the binding from javascript" do
      before do
        cxt['one'] = nil
        GC.start(full_mark: true, immediate_sweep: true)
      end
      it "can now garbage collect the ruby object" do
        expect(@ref.object).to be_nil
      end
    end
  end

  describe "proxying a JavaScript object into Ruby" do
    before do
      V8::C::V8::SetFlagsFromString("--expose_gc")
      @object = cxt.eval('(function() {return this.a = new Object()})();');
      @compare = cxt.eval('(function (object) {return this.a === object;})')
    end
    after { V8::C::V8::SetFlagsFromString("") }
    it "maintains identity in Ruby" do
      expect(@object).to be cxt['a']
      expect(@compare.call(@object)).to be_truthy
    end
    describe "removing references from JavaScript" do
      before do
        cxt.eval('delete this.a;')
        @id = cxt.eval('(function (object) { return object; })')
        cxt.isolate.native.RequestGarbageCollectionForTesting()
      end
      it "still holds onto the JavaScript Object" do
        expect(@id.call(@object)).to be @object
      end

      describe "removing all references from Ruby" do
        before do
          cxt.enter do
            V8::C::Handle::New(@object.native).SetWeak(proc { @did_finalize = true})
          end
          @result = {}
          ObjectSpace.define_finalizer(@object, proc { @did_finalize_ruby_peer = true })
          @object = nil
          GC.start(full_mark: true, immediate_sweep: true)
          cxt.isolate.native.RequestGarbageCollectionForTesting()
          cxt.isolate.native.__EachV8Finalizer__(&:call)
        end
        it "garbage collects the V8 object after the Ruby object is garbage collected" do
          expect(@did_finalize_ruby_peer).to eq true
          expect(@did_finalize).to eq true
        end
      end
    end
  end
end
