module V8
  ##
  # A completely encapsulated JavaScript virtual machine. No state of
  # any kind is shared between isolates.
  #
  # The isolate is the primany API for understanding and configuring
  # the runtime profile of the JavaScript interpreter. For example, it
  # is through the isolate that you can take heap and stack samples
  # for memory and cpu profiling. It is also through the isolate that
  # you can place restraints on the amount of memory and CPU that your
  # JavaScript execution will consume.
  #
  # An isolate can have many exceutions contexts that each have their
  # own scopes, and so significant resource savings can be had by
  # reusing the same isolate for multiple contexts.
  class Isolate

    # @!attribute [r] native
    #   @return [V8::C::Isolate] the underlying C++ object
    attr_reader :native

    ##
    # Create an new isolate.
    def initialize()
      @native = V8::C::Isolate::New()
    end

    ##
    # Terminates any currently executing JavaScript in this Isolate.
    #
    # This will throw an uncatchable exception inside the JavaScript
    # code which will forcibly unwind the stack.
    #
    # @return [NilClass] nil
    def terminate_execution!
      @native.TerminateExecution()
    end

    ##
    # Returns true if execution is terminating, but there are still
    # JavaScript frames left on the stack.
    #
    # @return [TrueClass | FalseClass] whether isolate is terminating execution
    def execution_terminating?
      @native.IsExecutionTerminating()
    end

    ##
    # Cancels any scheduled termination of JavaScript. Not really sure
    # why you would want to do this, but if you figure out a use-case,
    # would love for you to document it here.
    #
    # @return [NilClass] nil
    def cancel_terminate_execution!
      @native.CancelTerminateExecution()
    end
  end
end
