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

    context "Memory Management" do
      it "holds a hard reference to any ruby object which is linked to a javascript proxy" do
        rb_object = Object.new
        check_not_finalized(rb_object)
        subject.register_javascript_proxy c::Object::New(), :for => rb_object
        rb_object = nil
      end

      it "releases the hard reference if its corresponding javascript object has been garbage collected" do
          rb_object = Object.new
          js_proxy = c::Object::New()
          check_finalized(rb_object)
          subject.register_javascript_proxy js_proxy, :for => rb_object
          rb_object = nil
          ruby_gc do
            v8_gc()
          end
      end
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

    context "Memory Management" do

      it "holds a hard reference to any JavaScript object which is linked to a Ruby proxy" do
        js_object = c::Object::New()
        check_not_finalized(js_object)
        subject.register_ruby_proxy Object.new, :for => js_object
        js_object = nil
      end

      it "clears any strong references to the JavaScript object when it's Ruby proxy is garbage collected" do
        js_object = c::Object::New()
        rb_proxy = Object.new
        subject.register_ruby_proxy rb_proxy, :for => js_object
        check_finalized(js_object)
        js_object = rb_proxy = nil
        ruby_gc do
          v8_gc
        end
      end
    end
  end

  private

  def finalize(object_id)
    @finalized ||= {}
    @finalized[object_id] = true
  end

  def check_finalized(object)
    @finalized ||= {}
    ObjectSpace.define_finalizer(object, method(:finalize))
    id_to_check = object.object_id
    object = nil
    afterwards do
      @finalized[id_to_check].should be_true
    end
  end

  def check_not_finalized(object)
    @finalized ||= {}
    ObjectSpace.define_finalizer(object, method(:finalize))
    id_to_check = object.object_id
    object = nil
    afterwards do
      @finalized[id_to_check].should be_false
    end
  end

  def afterwards(&block)
    @after ||= []
    @after << block if block_given?
  end

  after do
    ruby_gc do
      @after.each(&:call) if @after
    end
  end

end
