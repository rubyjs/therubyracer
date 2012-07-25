require 'spec_helper'
require 'bigdecimal'
describe V8::Conversion do

  let(:cxt) { V8::Context.new }

  it "can embed BigDecimal values" do
    cxt['big'] = BigDecimal.new('1.1')
    cxt['big'].should eql BigDecimal.new('1.1')
  end

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

  context "::Fixnum" do
    context "for 32-bit numbers" do
      it "should convert positive integer" do
        cxt['fixnum_a'] = 0xFFFFFFFF
        cxt['fixnum_a'].should == 0xFFFFFFFF
        cxt['fixnum_a'].should be_instance_of(Fixnum)
      end

      it "should convert negative integer" do
        cxt['fixnum_b'] = -0xFFFF
        cxt['fixnum_b'].should == -0xFFFF
        cxt['fixnum_b'].should be_instance_of(Fixnum)
      end
    end

    context "for 64-bit numbers" do
      it "should convert positive integer" do
        cxt['fixnum_c'] = 0x100000000
        cxt['fixnum_c'].should == 0x100000000
      end

      it "should convert negative integer" do
        cxt['fixnum_d'] = -0xFFFFFFFF
        cxt['fixnum_d'].should == -0xFFFFFFFF
      end
    end

  end
end
