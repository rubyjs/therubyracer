#include "rr.h"

namespace rr {
  void Template::Init() {
    ClassBuilder("Template");
    ObjectTemplate::Init();
    FunctionTemplate::Init();
  }

  void ObjectTemplate::Init() {
    ClassBuilder("ObjectTemplate", "Template").
      store(&Class);
  }

  void FunctionTemplate::Init() {
    ClassBuilder("FunctionTemplate", "Template").
      defineMethod("New", &New).
      store(&Class);
  }

  VALUE FunctionTemplate::New(int argc, VALUE argv[], VALUE self) {
    VALUE code; VALUE data; VALUE signature;
    rb_scan_args(argc, argv, "03", &code, &data, &signature);
    return FunctionTemplate(v8::FunctionTemplate::New());
    // InvocationCallback callback(code, data);
    // return FunctionTemplate(v8::FunctionTemplate::New(callback, callback.data, Signature(signature)));
  }

  // /** Creates a function template.*/
  // static Local<FunctionTemplate> New(
  //     InvocationCallback callback = 0,
  //     Handle<Value> data = Handle<Value>(),
  //     Handle<Signature> signature = Handle<Signature>());
  // /** Returns the unique function instance in the current execution context.*/
  // Local<Function> GetFunction();
  //
  // /**
  //  * Set the call-handler callback for a FunctionTemplate.  This
  //  * callback is called whenever the function created from this
  //  * FunctionTemplate is called.
  //  */
  // void SetCallHandler(InvocationCallback callback,
  //                     Handle<Value> data = Handle<Value>());
  //
  // /** Get the InstanceTemplate. */
  // Local<ObjectTemplate> InstanceTemplate();
  //
  // /** Causes the function template to inherit from a parent function template.*/
  // void Inherit(Handle<FunctionTemplate> parent);
  //
  // /**
  //  * A PrototypeTemplate is the template used to create the prototype object
  //  * of the function created by this template.
  //  */
  // Local<ObjectTemplate> PrototypeTemplate();
  //
  //
  // /**
  //  * Set the class name of the FunctionTemplate.  This is used for
  //  * printing objects created with the function created from the
  //  * FunctionTemplate as its constructor.
  //  */
  // void SetClassName(Handle<String> name);
  //
  // /**
  //  * Determines whether the __proto__ accessor ignores instances of
  //  * the function template.  If instances of the function template are
  //  * ignored, __proto__ skips all instances and instead returns the
  //  * next object in the prototype chain.
  //  *
  //  * Call with a value of true to make the __proto__ accessor ignore
  //  * instances of the function template.  Call with a value of false
  //  * to make the __proto__ accessor not ignore instances of the
  //  * function template.  By default, instances of a function template
  //  * are not ignored.
  //  */
  // void SetHiddenPrototype(bool value);
  //
  // /**
  //  * Sets the ReadOnly flag in the attributes of the 'prototype' property
  //  * of functions created from this FunctionTemplate to true.
  //  */
  // void ReadOnlyPrototype();
  //
  // /**
  //  * Returns true if the given object is an instance of this function
  //  * template.
  //  */
  // bool HasInstance(Handle<Value> object);

}