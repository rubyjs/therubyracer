require Pathname(__FILE__).dirname.join('../spec/spec_helper')

module V8::MemSpec
  
  def self.included(cls)
    cls.class_eval do
      include V8::ExtSpec
      before(:all) {V8::C::V8::SetFlagsFromString("--expose-gc")}
    end
  end
  
  def ruby_gc
    current = GC.stress
    GC.stress = true
    yield
  ensure
    GC.stress = current
  end

  def v8_gc
    while !c::V8::IdleNotification();end
    v8_eval('gc()', 'gc.js')
  end
end