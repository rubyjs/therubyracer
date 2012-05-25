require 'spec_helper'

describe V8::C::Locker do
  it "can lock and unlock the VM" do
    pending "need to figure out how to wrap rspec methods"
    V8::C::Locker::IsLocked().should be_false
    V8::C::Locker() do
      V8::C::Locker::IsLocked().should be_true
      V8::C::Unlocker() do
        V8::C::Locker::IsLocked().should be_false
      end
    end
    V8::C::Locker::IsLocked().should be_false
  end

  it "properly unlocks if an exception is thrown inside a lock block" do
    pending "need to figure out how to wrap rspec methods"
    begin
      V8::C::Locker() do
        raise "boom!"
      end
    rescue
      V8::C::Locker::IsLocked().should be_false
    end
  end
end