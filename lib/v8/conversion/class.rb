class V8::Conversion
  module Class
    include V8::Util::Weakcell

    def to_v8
      weakcell(:v8_constructor) do
        V8::C::FunctionTemplate::New()
      end.GetFunction()
    end
  end
end