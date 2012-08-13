module V8
  # capture 99 stack frames on exception with normal details.
  # You can adjust these values for performance or turn of stack capture entirely
  V8::C::V8::SetCaptureStackTraceForUncaughtExceptions(true, 99, V8::C::StackTrace::kOverview)
  class Error < StandardError
    include Enumerable
    attr_reader :value, :cause, :javascript_backtrace

    def initialize(message, value, javascript_backtrace, cause = nil)
      super(message)
      @value = value
      @cause = cause
      @javascript_backtrace = javascript_backtrace
    end

    def causes
      [].tap do |causes|
        current = self
        until current.nil? do
          causes.push current
          current = current.respond_to?(:cause) ? current.cause : nil
        end
      end
    end

    def root_cause
      causes.last
    end

    def in_javascript?
      causes.last.is_a? self.class
    end

    def in_ruby?
      !in_javascript?
    end

    def multilingual_backtrace
      causes.reduce(:backtrace => [], :ruby => -1, :javascript => -1) { |accumulator, cause|
        ruby_frames = cause.backtrace[0..accumulator[:ruby]]
        accumulator[:backtrace].unshift *ruby_frames
        accumulator[:ruby] -= ruby_frames.length
        if cause.respond_to?(:javascript_backtrace)
          javascript_frames = cause.javascript_frames[0, accumulator[:javascript]].map(&:to_s)
          accumulator[:backtrace].unshift *javascript_frames
          accumulator[:javascript] -= javascript_frames.length
        end
      }[:backtrace]
    end

    module Try
      def try
        V8::C::TryCatch() do |trycatch|
          result = yield
          if trycatch.HasCaught()
            raise V8::Error(trycatch)
          else
            result
          end
        end
      end
    end

    module Protect
      def protect
        yield
      rescue Exception => e
        error = V8::C::Exception::Error(e.message)
        error.SetHiddenValue("rr::Cause", V8::C::External::New(e))
        V8::C::ThrowException(error)
      end
    end

  end

  def self.Error(trycatch)
    exception = trycatch.Exception()
    value = exception.to_ruby
    cause = nil
    message = if !exception.kind_of?(V8::C::Value)
      exception.to_s
    elsif exception.IsNativeError()
      if cause = exception.GetHiddenValue("rr::Cause")
        cause = cause.Value()
      end
      exception.Get("message").to_ruby
    elsif exception.IsObject()
      value['message'] || value.to_s
    else
      value.to_s
    end
    javascript_backtrace = V8::StackTrace.new(trycatch.Message().GetStackTrace())
    V8::Error.new(message, value, javascript_backtrace, cause)
  end
  const_set :JSError, Error
end