
require 'spec_helper'

describe V8::C::Handle do

  it "can get a new context" do
    cxt = c::Handle::NewContext()
    cxt.IsEmpty().should be_false
    cxt.MakeWeak()
    gc
    cxt.IsNearDeath().should be_true
    # cxt.Clear()
    cxt.IsEmpty().should be_true
  end

  def c
    V8::C
  end
  
  def gc
    while (!V8::C::V8::IdleNotification()); end
    yield if block_given?
  end
end