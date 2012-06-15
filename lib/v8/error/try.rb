class V8::Error
  module Try
    def try
      context = V8::Context.current
      V8::C::TryCatch() do |trycatch|
        result = yield
        if trycatch.HasCaught()
          V8::Error(trycatch.Exception())
        else
          result
        end
      end
    end
  end
end