require 'spec_helper'

describe V8::Portal::Proxies do
  include V8::ExtSpec


  context "for Ruby objects which are embedded into javascript" do

    it "allows you to resolve the Ruby object's JavaScript proxy" do
      js_proxy = c::Object::New()
      rb_object = Object.new
      subject.register_javascript_proxy js_proxy, :for => rb_object
      subject.rb_object_2_js_proxy(rb_object).should be(js_proxy)
      subject.js_proxy_2_rb_object(js_proxy).should be(rb_object)
    end

    it "requires a Ruby object which is the actual object that is proxied" do
      expect {subject.register_javascript_proxy c::Object::New()}.should raise_error(ArgumentError)
    end

    it "can only register proxies which are low-level JavaScript objects" do
      expect {subject.register_javascript_proxy Object.new, :for => Object.new}.should raise_error(ArgumentError)
    end

    it "is only allowed to have a single JavaScript proxy" do
      rb_object = Object.new
      subject.register_javascript_proxy c::Object::New(), :for => rb_object
      expect {subject.register_javascript_proxy c::Object::New(), :for => rb_object}.should raise_error(V8::Portal::Proxies::DoubleProxyError)
    end
  end

  context "for a JavaScript objects which are embedded into Ruby" do

    it "allows you to resolve the JavaScript object's Ruby proxy" do
      rb_proxy = Object.new
      js_object = c::Object::New()
      subject.register_ruby_proxy rb_proxy, :for => js_object
      subject.js_object_2_rb_proxy(js_object).should be(rb_proxy)
      subject.rb_proxy_2_js_object(rb_proxy).should be(js_object)
    end

    it "requires a JavaScript low level javascript object as the actual object that is proxied" do
      expect {subject.register_javascript_proxy Object.new, :for => c::Object::New()}.should raise_error(ArgumentError)
    end

    it "will not a proxy twice if the proxy creator block actually registers the proxy inside it" do
      target = Object.new
      proxy = c::Object::New()
      expect {subject.rb2js(target) do |object|
        subject.register_javascript_proxy(proxy, :for => object)
        c::Object::New()
      end}.should_not raise_error
      subject.rb2js(target).should be(proxy)
    end

    context "looking up a Ruby object from a random JavaScript object" do
      it "checks first if it's a native Ruby object with a javascript proxy" do
        target = Object.new
        proxy = c::Object::New()
        subject.register_javascript_proxy proxy, :for => target
        subject.js2rb(proxy).should be(target)
      end
      it "then sees if maybe it's a native JavaScript that has a Ruby proxy" do
        target = c::Object::New()
        proxy = Object.new
        subject.register_ruby_proxy proxy, :for => target
        subject.js2rb(target).should be(proxy)
      end
      it "will assume that it is a native JavaScript object that needs a Ruby proxy if no corresponding Ruby object can be found" do
        js = c::Object::New()
        proxy = subject.js2rb(js) do |target|
          {:target => target}
        end
        subject.js2rb(js).should be(proxy)
      end
    end

    context "looking up a JavaScript object from a random Ruby object" do
      it "checks first if it's a native JavaScript object with a Ruby proxy" do
        target = c::Object::New()
        proxy = Object.new
        subject.register_ruby_proxy proxy, :for => target
        subject.rb2js(proxy).should be(target)
      end

      it "then sees if maybe it's a native Ruby object that has a JavaScript proxy" do
        target = Object.new
        proxy = c::Object::New()
        subject.register_javascript_proxy proxy, :for => target
        subject.rb2js(target).should be(proxy)
      end

      it "assumes that it is a native Ruby object that needs a JavaScript proxy if no corresponding JavaScript object can be found" do
        rb = Object.new
        proxy = nil
        js = subject.rb2js(rb) do |target|
          target.should be(rb)
          proxy = c::Object::New()
        end
        proxy.should_not be_nil
        js.should be(proxy)
        subject.rb2js(rb).should be(proxy)
      end
    end
  end
end
