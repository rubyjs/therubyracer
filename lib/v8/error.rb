module V8
  class Error < StandardError
    attr_reader :value
    def initialize(message, value)
      super(message)
      @value = value
    end
  end
  const_set :JSError, Error

  def self.Error(exception)
    value = exception.to_ruby
    if !exception.kind_of?(V8::C::Value)
      raise V8::Error.new(exception.to_s, value)
    elsif exception.IsNativeError()
      if football = exception.GetHiddenValue("rr::Football")
        raise football.Value()
      else
        raise V8::Error.new(exception.Get("message").to_ruby, value)
      end
    else
      raise V8::Error.new(exception.ToString().to_ruby, value)
    end
  end
end