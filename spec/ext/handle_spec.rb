
require 'spec_helper'

describe V8::C::Handle do

  before(:all) do
    V8::C::V8::SetFlagsFromString("--expose-gc")
    @cxt = V8::Context.new
  end

  def c
    V8::C
  end
  
  def gc
    @cxt.eval('gc()');
  end
end