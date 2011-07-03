
module V8::ExtSpec

  def self.included(object)
    object.class_eval do
      before do
        @lock = c::Locker.new
        @cxt = c::Context::New()
        @cxt.Enter()
      end
      after do
        @cxt.Exit()
        @cxt.Dispose()
        @lock.delete
      end
    end
  end

  def v8_eval(script, sourcename = "<eval>")
    c::Script::New(c::String::New(script), c::String::New(sourcename)).Run()
  end

  def c
    V8::C
  end

end