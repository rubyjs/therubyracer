
module V8::ExtSpec
  
  def self.included(object)
    object.class_eval do
      before(:all) {c::V8::SetFlagsFromString("--expose-gc")}
      before do
        @cxt = c::Context::New()
        @cxt.Enter()
      end
      after do
        @cxt.Exit()
        @cxt.Dispose()
      end
    end
  end
  
  def v8_eval(script, sourcename = "<eval>")
    c::Script::New(c::String::New(script), c::String::New(sourcename)).Run()
  end

  def c
    V8::C
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