
module V8
  class JSError < StandardError
    attr_reader :source_name, :source_line, :line_number, :javascript_stacktrace
    
    def initialize(try)
      msg = try.Message()
      @source_name = To.rb(msg.GetScriptResourceName())
      @source_line = To.rb(msg.GetSourceLine())
      @line_number = To.rb(msg.GetLineNumber())
      @javascript_stacktrace = To.rb(try.StackTrace())
      super("#{To.rb(msg.Get())}: #{@source_name}:#{@line_number}")
    end
    
  end
end