module V8Matchers
  extend RSpec::Matchers::DSL

  matcher :eq_just do |expected|
    match do |maybe|
      return false unless maybe.IsJust

      maybe.FromJust == expected
    end
  end

  matcher :be_successful do
    match do |maybe|
      return false unless maybe.IsJust

      maybe.FromJust
    end
  end

  matcher :strict_eq do |expected|
    match do |object|
      if object.respond_to? 'IsJust'
        return false unless object.IsJust
        object = object.FromJust
      end

      object.StrictEquals(expected)
    end
  end

  matcher :v8_eq do |expected|
    match do |object|
      if object.respond_to? 'IsJust'
        return false unless object.IsJust
        object = object.FromJust
      end

      object.Equals(expected)
    end
  end
end

RSpec.configure do |c|
  c.include V8Matchers
end
