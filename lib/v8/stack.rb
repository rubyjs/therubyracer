
module V8

  class StackTrace
    include Enumerable

    def initialize(to, native)
      @to = to
      @native = native
    end

    def length
      @native.GetFrameCount()
    end

    def each
      for i in 0..length - 1
        yield V8::StackFrame.new(@to, @native.GetFrame(i))
      end
    end

    def to_s
      map {|f|"at #{f}"}.join("\n")
    end
  end

  class StackFrame

    def initialize(portal, native)
      @to = portal
      @native = native
    end

    def script_name
      @to.rb(@native.GetScriptName())
    end

    def function_name
      @to.rb(@native.GetFunctionName())
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
      if @native.GetFunctionName()
        "#{function_name} (#{script_name}:#{line_number}:#{column})"
      else
        "#{script_name}:#{line_number}:#{column}"
      end
    end
  end
end