
module V8::ExtSpec

  def self.included(object)
    object.class_eval do
      before do
        @lock = c::Locker.new
        @ctx = c::Context::New()
        @ctx.Enter()
      end
      after do
        @ctx.Exit()
        @ctx.Dispose()
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