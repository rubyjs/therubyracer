
module V8
  class JSError < StandardError
    attr_reader :value, :boundaries

    def initialize(try, to)
      @to = to
      begin
        super(initialize_unsafe(try))
      rescue Exception => e
        @boundaries = Boundary.new(:rbframes => e.backtrace)
        @value = e
        super("BUG! please report. JSError#initialize failed!: #{e.message}")
      end
    end

    def initialize_unsafe(try)
      message = nil
      ex = @to.rb(try.Exception())
      @boundaries = [Boundary.new(:rbframes => caller(3), :jsframes => parse_js_frames(try))]
      if V8::Object === ex
        if msg = ex['message']
          message = msg
        else
          message = ex.to_s
        end
        if cause = ex.instance_variable_get(:@native).GetHiddenValue("TheRubyRacer::Cause")
          if !cause.IsEmpty()
            prev = cause.Value()
            if prev.kind_of?(JSError)
              @boundaries.concat prev.boundaries
              @value = prev.value
            else
              @value = prev
              @boundaries.concat [Boundary.new(:rbframes => prev.backtrace)]
            end
          else
            @value = ex
          end
        end
      else
        @value = ex
        message = ex.to_s
        @boundaries.first.jsframes << 'at [???].js'
      end
      return message
    end

    def in_ruby?
      @value.kind_of?(Exception)
    end

    def in_javascript?
      !in_ruby?
    end

    def backtrace(*modifiers)
      trace_framework = modifiers.include?(:framework)
      trace_ruby = modifiers.length == 0 || modifiers.include?(:ruby)
      trace_javascript = modifiers.length == 0 || modifiers.include?(:javascript)
      mixed = []
      rbcontext = []
      jscontext = []
      @boundaries.each do |b|
        rbframes = b.rbframes.dup
        rbcontext.reverse_each do |frame|
          if frame == rbframes.last
            rbframes.pop
          else
            break
          end
        end if trace_ruby
        jsframes = b.jsframes.dup
        jscontext.reverse_each do |frame|
          if frame == jsframes.last
            jsframes.pop
          else
            break
          end
        end if trace_javascript
        rbcontext = b.rbframes
        jscontext = b.jsframes
        rbframes.reject! {|f| f =~ /lib\/v8\/\w+\.rb/} unless trace_framework
        mixed.unshift(*rbframes) if trace_ruby
        mixed.unshift(*jsframes) if trace_javascript
      end
      return mixed
    end

    def parse_js_frames(try)
      raw = @to.rb(try.StackTrace())
      if raw && !raw.empty?
        raw.split("\n")[1..-1].tap do |frames|
          frames.each {|frame| frame.strip!.chomp!(",")}
        end
      else
        []
      end
    end

    class Boundary
      attr_reader :rbframes, :jsframes

      def initialize(frames = {})
        @rbframes = frames[:rbframes] || []
        @jsframes = frames[:jsframes] || []
      end
    end
  end
  #deprecated -- use JSError
  JavasriptError = JSError
end