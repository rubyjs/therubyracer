
module V8

  class StackTrace
    include Enumerable

    def initialize(native)
      @native = native
    end

    def length
      @native ? @native.GetFrameCount() : 0
    end

    def each
      return unless @native
      for i in 0..length - 1
        yield V8::StackFrame.new(@native.GetFrame(i))
      end
    end

    def to_s
      @native ? map(&:to_s).join("\n") : ""
    end
  end

  class StackFrame

    def initialize(native)
      @context = V8::Context.current
      @native = native
    end

    def script_name
      @context.to_ruby(@native.GetScriptName())
    end

    def function_name
      @context.to_ruby(@native.GetFunctionName())
    end

    def line_number
      @native.GetLineNumber()
    end

    def column
      @native.GetColumn()
    end

    def eval?
      @native.IsEval()
    end

    def constructor?
      @native.IsConstructor()
    end

    def to_s
      "at " + if !function_name.empty?
        "#{function_name} (#{script_name}:#{line_number}:#{column})"
      else
        "#{script_name}:#{line_number}:#{column}"
      end
    end
  end
end