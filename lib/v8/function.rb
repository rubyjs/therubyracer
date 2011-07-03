module V8
  class Function < V8::Object

    def methodcall(thisObject, *args)
      err = nil
      return_value = nil
      @portal.open do |to|
        C::TryCatch.try do |try|
          this = to.v8(thisObject)
          return_value = to.rb(@native.Call(this, to.v8(args)))
          err = JSError.new(try, to) if try.HasCaught()
        end
      end
      raise err if err
      return return_value
    end

    def call(*args)
      @portal.open do
        self.methodcall(@portal.context.native.Global(), *args)
      end
    end

    def new(*args)
      @portal.open do |to|
        to.rb(@native.NewInstance(to.v8(args)))
      end
    end

    def name
      @portal.open do |to|
        to.rb(@native.GetName())
      end
    end

    def name=(name)
      name.tap do
        @portal.open do |to|
          @native.SetName(to.v8(name))
        end
      end
    end
  end
end