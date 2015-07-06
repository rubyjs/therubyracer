module V8
  class RetainedObjects
    def initialize
      @counts = {}
    end

    def add(object)
      if @counts[object]
        @counts[object] += 1
      else
        @counts[object] = 1
      end
    end

    def remove(object)
      if count = @counts[object]
        if count <= 1
          @counts.delete object
        else
          @counts[object] -= 1
        end
      end
    end

    def retaining?(object)
      !!@counts[object]
    end
  end
end
