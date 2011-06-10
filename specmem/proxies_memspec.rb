require 'spec_helper'

describe V8::Portal::Proxies do
  include V8::MemSpec
  
  context "A Ruby object embedded into JavaScript" do
    it "holds a hard reference to any ruby object which is linked to a javascript proxy" do
      subject.register_javascript_proxy c::Object::New(), :for => Object.new
      ruby_gc do
        subject.should_not be_empty
      end
    end

    it "releases the hard reference if its corresponding javascript object has been garbage collected" do
      ruby_gc do
        rb_object = Object.new
        js_proxy = c::Object::New()
        subject.should be_empty
        subject.register_javascript_proxy js_proxy, :for => rb_object
        rb_object = nil
        subject.should_not be_empty
        v8_gc()
      end
      subject.should be_empty
    end
  end

  context "A JavaScript object embedded into Ruby" do
    it "holds a hard reference to any JavaScript object which is linked to a Ruby proxy" do
      proxy = Object.new
      subject.register_ruby_proxy proxy, :for => c::Object::New()
      ruby_gc do
        subject.should_not be_empty
      end
    end

    it "clears any strong references to the JavaScript object when it's Ruby proxy is garbage collected" do
      subject.register_ruby_proxy Object.new, :for => c::Object::New()
      subject.should_not be_empty
      ruby_gc do
        v8_gc
        GC.start
        v8_gc
      end
      subject.should be_empty
    end
  end

end
