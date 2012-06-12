require 'weakref'
module V8
  module Util
    module Weakcell
      def weakcell(name, &block)
        unless storage = instance_variable_get("@#{name}")
          storage = instance_variable_set("@#{name}", Storage.new)
        end
        storage.access(&block)
      end
      class Storage
        def access(&block)
          if @ref
            @ref.__getobj__
          else
            populate(block)
          end
        rescue RefError
          populate(block)
        end

        private

        def populate(block)
          occupant = block.call()
          @ref = WeakRef.new(occupant)
          return occupant
        end
      end
    end
  end
end