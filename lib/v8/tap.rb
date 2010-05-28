
unless Object.method_defined?(:tap)
  class Object
    def tap
      yield self
      return self
    end
  end
end
