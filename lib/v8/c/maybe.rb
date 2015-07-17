module V8
  module C
    ##
    # Some V8 API methods return a `Maybe` value that force you to check
    # if the operation failed or not before accessing the underlying
    # value. This implements the `v8::Maybe` and `v8::MaybeLocal` apis,
    # and is returned by those low-level C methods:
    #
    #   maybe = object.Get(context, key)
    #   if maybe.IsJust()
    #     puts maybe.class #=> V8::C::Maybe::Just
    #     puts "the value is #{maybe.FromJust()}"
    #   else
    #     puts maybe.class #=> V8::C::Maybe::Nothing
    #     puts "operation failed!"
    #   end
    #
    # Note: Instances of `V8::C::Maybe` are only ever created by the C
    # extension, and should not ever be instantiated from Ruby code.
    class Maybe

      ##
      # If true, then this is an instance of `V8::C::Just`, and it does
      # wrap a value
      def IsJust()
        false
      end

      ##
      # If true, then this is an instance of `V8::C::Nothing`, and it
      # does not contain a value. Any attempt to access the value will
      # raise an error.
      def IsNothing()
        false
      end

      ##
      # A Maybe that *does* contain a value.
      class Just < Maybe
        def initialize(value)
          @value = value
        end

        def IsJust()
          true
        end

        def FromJust()
          @value
        end
      end

      ##
      # A Maybe that *does* not contain any value
      class Nothing < Maybe

        def IsNothing()
          true
        end

        def FromJust()
          fail PendingExceptionError, "no value was available because an exception is pending in this context"
        end
      end

      ##
      # Singleton instance of Nothing.
      #
      # We only need one instance of Nothing because there is no
      # additional state apart from its nothingness.
      def self.nothing
        @nothing ||= Nothing.new
      end
    end

    class PendingExceptionError < StandardError;end
  end
end
