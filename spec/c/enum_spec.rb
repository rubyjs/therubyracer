require 'c_spec_helper'

describe V8::C do
  describe 'AccessControl' do
    it 'has all enum values' do
      expect(V8::C::AccessControl.const_defined? 'DEFAULT').to be true
      expect(V8::C::AccessControl.const_defined? 'ALL_CAN_READ').to be true
      expect(V8::C::AccessControl.const_defined? 'ALL_CAN_WRITE').to be true
      expect(V8::C::AccessControl.const_defined? 'PROHIBITS_OVERWRITING').to be true
    end
  end

  describe 'PropertyAttribute' do
    it 'has all enum values' do
      expect(V8::C::PropertyAttribute.const_defined? 'None').to be true
      expect(V8::C::PropertyAttribute.const_defined? 'ReadOnly').to be true
      expect(V8::C::PropertyAttribute.const_defined? 'DontEnum').to be true
      expect(V8::C::PropertyAttribute.const_defined? 'DontDelete').to be true
    end
  end
end
