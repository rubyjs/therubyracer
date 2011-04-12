
require 'spec_helper'

describe "Memory:" do
  context "A JavaScript Object reflected into ruby" do
    before(:all) {c::V8::SetFlagsFromString("--expose-gc")}
    before do
      @cxt = c::Context::New()
      @cxt.Enter()
    end
    
    after do
      @cxt.Exit()
    end

    it "has a strong reference from the ruby side" do
      handle = c::Handle::New(object = c::Object::New())
      collected = false
      handle.MakeWeak() {collected = true; puts "yo"}
      v8_gc
      collected.should_not be_true
      handle.IsDead().should be_false
    end
    
    it "will be garbarge collected if there are no more ruby references and is weakly reachable from V8" do
      handle = c::Handle::New(object = c::Object::New())
      object = nil
      collected = false
      handle.MakeWeak() {collected = true}
      ruby_gc do
        v8_gc
        collected.should be_true
        handle.IsDead().should be_true
      end
    end
    
    private 

    def v8_gc
      c::Script::New(c::String::New("gc()"), c::String::New("gc.js")).Run()
    end
    
    def ruby_gc
      GC.stress = true
      yield if block_given?
    ensure
      GC.stress = false
    end
    
    def c
      V8::C
    end
  end
end