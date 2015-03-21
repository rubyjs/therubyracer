require 'c_spec_helper'

describe V8::C::Isolate do
  before(:each) { cleanup_isolates }

  it 'can create a new isolate' do
    expect(V8::C::Isolate.New).to be
  end

  it 'can be tested for equality' do
    isolate_one = V8::C::Isolate.New
    isolate_two = V8::C::Isolate.New

    expect(isolate_one.Equals(isolate_one)).to eq true
    expect(isolate_one.Equals(isolate_two)).to eq false
  end

  describe 'GetCurrent' do
    it 'can get the current isolate for the thread' do
      isolate = V8::C::Isolate.New
      isolate.Enter

      expect(V8::C::Isolate.GetCurrent.Equals(isolate)).to eq true
    end

    it 'returns nil if there are no entered isolates' do
      expect(V8::C::Isolate.GetCurrent).to be_nil
    end

    it 'is different accross threads' do
      V8::C::Isolate.New.Enter

      Thread.new do
        expect(V8::C::Isolate.GetCurrent).to be_nil
      end.join
    end
  end

  it 'can enter and exit multiple times' do
    isolate_one = V8::C::Isolate.New
    isolate_two = V8::C::Isolate.New

    isolate_one.Enter
    isolate_one.Enter
    isolate_two.Enter

    expect(V8::C::Isolate.GetCurrent.Equals(isolate_two)).to eq true
    isolate_two.Exit

    expect(V8::C::Isolate.GetCurrent.Equals(isolate_one)).to eq true
    isolate_one.Exit

    expect(V8::C::Isolate.GetCurrent.Equals(isolate_one)).to eq true
    isolate_one.Exit
  end
end
