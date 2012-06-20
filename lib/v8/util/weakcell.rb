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
            @ref.object || populate(block)
          else
            populate(block)
          end
        end

        private

        def populate(block)
          occupant = block.call()
          @ref = Ref::WeakReference.new(occupant)
          return occupant
        end
      end
    end
  end
end