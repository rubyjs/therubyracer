require 'spec_helper'

# NOTE: This was written to reproduce a bug where V8::Conversion would load the wrong class
# when inside of an eigen class scope.

# We use Set because ::Set is a existing class and V8::Conversion::Class::Set also exists
require "set"

describe "Class scope" do
  it "doesn't try to use V8::Conversion::Class::* as root objects" do
    klass = Class.new do
      class << self
        def test
          Set.new
        end
      end
    end

    klass.test.should be_instance_of(::Set)
  end
end