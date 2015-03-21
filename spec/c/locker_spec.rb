require 'c_spec_helper'

describe V8::C::Locker do
  let(:isolate) { V8::C::Isolate.New }

  it 'can lock and unlock the VM' do
    expect(V8::C::Locker::IsLocked(isolate)).to eq false

    V8::C::Locker(isolate) do
      expect(V8::C::Locker::IsLocked(isolate)).to eq true

      V8::C::Unlocker(isolate) do
        expect(V8::C::Locker::IsLocked(isolate)).to eq false
      end
    end

    expect(V8::C::Locker::IsLocked(isolate)).to eq false
  end

  it 'properly unlocks if an exception is thrown inside a lock block' do
    begin
      V8::C::Locker(isolate) do
        raise 'boom!'
      end
    rescue
      expect(V8::C::Locker::IsLocked(isolate)).to eq false
    end
  end

  it 'properly re-locks if an exception is thrown inside an un-lock block' do
    V8::C::Locker(isolate) do
      begin
        V8::C::Unlocker(isolate) do
          raise 'boom!'
        end
      rescue
        expect(V8::C::Locker::IsLocked(isolate)).to eq true
      end
    end
  end
end
