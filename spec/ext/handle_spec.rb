
require 'spec_helper'

describe V8::C::Handle do

  before(:all) do
    V8::C::V8::SetFlagsFromString("--expose-gc")
    @cxt = V8::Context.new
  end

  it "can get a new context" do
    cxt = c::Handle::NewContext()
    cxt.IsEmpty().should be_false
    cxt.MakeWeak()
    gc
    # cxt.IsNearDeath().should be_true
    # cxt.Clear()
    cxt.IsEmpty().should be_true
  end

  def c
    V8::C
  end
  
  def gc
    @cxt.eval('gc()');
  end
end