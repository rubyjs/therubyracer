require 'spec_helper'
require 'redjs/load_specs'
module RedJS
  Context = V8::Context
  # Error = V8::JSError
end
describe V8::Context do
  it_behaves_like 'RedJS::Context'
end