
require 'spec_helper'

describe "Memory:" do
  include V8::ExtSpec
  context "A JavaScript Object reflected into Ruby" do

    before do
      @weakref_callback = WeakrefCallback.new
    end

    it "has a strong reference from the ruby side, which is not released until the Ruby reference goes away" do
      handle = c::Handle::New(object = c::Object::New())
      handle.MakeWeak(&@weakref_callback)
      ruby_gc do
        v8_gc
        @weakref_callback.should_not have_been_invoked
        handle.should_not be_dead
      end
      ruby_gc do
        object = nil
        v8_gc
        @weakref_callback.should have_been_invoked
        handle.should be_dead
      end
    end

    private

    class WeakrefCallback
      def to_proc
        method(:invoke).to_proc
      end

      def invoke
        @invoked = true
      end

      def has_been_invoked?
        @invoked
      end
    end
  end
end