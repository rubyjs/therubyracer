
require 'spec_helper'
require 'weakref'

describe "Memory Usage" do

  context "for low level C interface" do
    it "does not leak a context" do
      weak :context, V8::C::Context::New()
    end

    it "does not leak a simple object either" do
      cxt = V8::C::Context::New()
      cxt.Enter()
      begin
        weak :object, V8::C::Object::New()
      ensure
        cxt.Exit()
      end
    end
  end

  before(:all) {GC.stress = true}
  after(:all) {GC.stress = false}

  before do
    @refs = {}
  end

  after do
    GC.start
    while !V8::C::V8::IdleNotification();end
    GC.start
    while !V8::C::V8::IdleNotification();end 
    for name, ref in @refs
      if ref.weakref_alive?
        fail "refererc #{name} was not garbage collected"
      end
    end
  end

  private
  
  def weak(name, ref)
    @refs[name] = WeakRef.new(ref)
  end

end if GC.respond_to?(:stress=)