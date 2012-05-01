require 'spec_helper'

describe "A Very blunt test to make sure that we aren't doing stupid leaks" do
  before do
    @start_memory = process_memory
    GC.stress = true
  end
  
  after do
    GC.stress = false
  end
  it "won't leak the context" do
    500.times do
      V8::Context.new
    end
    gc_completely
    process_memory.should <= @start_memory * 1.05
  end

  def process_memory
    /\w*[ ]*#{Process.pid}[ ]*([.,\d]*)[ ]*([.,\d]*)[ ]*([\d]*)[ ]*([\d]*)/.match(`ps aux`)[4].to_i
  end

  def gc_completely
    
  end

end

