module V8
  class Date < V8::Object
    def value_of
      @context.to_ruby @native.ValueOf()
    end
  end
end
