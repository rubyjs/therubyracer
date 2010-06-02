
module V8
  module Jasmine
    class Context < V8::Context
      def initialize(*args)
        super(*args) do
          self.extend V8::Jasmine
          yield(self) if block_given?
        end
      end
    end
  end
end