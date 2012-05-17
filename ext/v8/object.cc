#include "rr.h"

namespace rr {

void Object::Init() {
  ClassBuilder("Object", "Value").
    defineSingletonMethod("New", &New).
    defineMethod("Set", &Set).
    defineMethod("ForceSet", &ForceSet).
    defineMethod("Get", &Get).
    defineMethod("GetPropertyAttributes", &GetPropertyAttributes).
    defineMethod("Has", &Has).
    defineMethod("Delete", &Delete).
    defineMethod("ForceDelete", &ForceDelete).
    defineMethod("SetAccessor", &SetAccessor).
    store(&Class);
  ClassBuilder("PropertyAttribute").
    defineEnumConst("None", v8::None).
    defineEnumConst("ReadOnly", v8::ReadOnly).
    defineEnumConst("DontEnum", v8::DontEnum).
    defineEnumConst("DontDelete", v8::DontDelete);
  ClassBuilder("AccessControl").
    defineEnumConst("DEFAULT", v8::DEFAULT).
    defineEnumConst("ALL_CAN_READ", v8::ALL_CAN_READ).
    defineEnumConst("ALL_CAN_WRITE", v8::ALL_CAN_WRITE).
    defineEnumConst("PROHIBITS_OVERWRITING", v8::PROHIBITS_OVERWRITING);
}


VALUE Object::New(VALUE self) {
  return Object(v8::Object::New());
}

//TODO: Allow setting of property attributes
VALUE Object::Set(VALUE self, VALUE key, VALUE value) {
  if (rb_obj_is_kind_of(key, rb_cNumeric)) {
    return Bool(Object(self)->Set(UInt32(key), Value(value)));
  } else {
    return Bool(Object(self)->Set(*Value(key), Value(value)));
  }
}

VALUE Object::ForceSet(VALUE self, VALUE key, VALUE value) {
  return Bool(Object(self)->ForceSet(Value(key), Value(value)));
}

VALUE Object::Get(VALUE self, VALUE key) {
  if (rb_obj_is_kind_of(key, rb_cNumeric)) {
    return Value(Object(self)->Get(UInt32(key)));
  } else {
    return Value(Object(self)->Get(*Value(key)));
  }
}

VALUE Object::GetPropertyAttributes(VALUE self, VALUE key) {
  return PropertyAttribute(Object(self)->GetPropertyAttributes(Value(key)));
}

VALUE Object::Has(VALUE self, VALUE key) {
  Object obj(self);
  if (rb_obj_is_kind_of(key, rb_cNumeric)) {
    return Bool(obj->Has(UInt32(key)));
  } else {
  return Bool(obj->Has(*String(key)));
  }
}

VALUE Object::Delete(VALUE self, VALUE key) {
  Object obj(self);
  if (rb_obj_is_kind_of(key, rb_cNumeric)) {
    return Bool(obj->Delete(UInt32(key)));
  } else {
    return Bool(obj->Delete(*String(key)));
  }
}

VALUE Object::ForceDelete(VALUE self, VALUE key) {
  return Bool(Object(self)->ForceDelete(Value(key)));
}


VALUE Object::SetAccessor(int argc, VALUE* argv, VALUE self) {
  VALUE name; VALUE get; VALUE set; VALUE data; VALUE settings; VALUE attribs;
  rb_scan_args(argc, argv, "24", &name, &get, &set, &data, &settings, &attribs);
  Accessor::Info accessor(get, set, data);
  return Bool(Object(self)->SetAccessor(String(name),
                                        accessor.Getter(),
                                        accessor.Setter(),
                                        accessor,
                                        AccessControl(settings),
                                        PropertyAttribute(attribs)));
}

//
// /**
//  * Returns an array containing the names of the enumerable properties
//  * of this object, including properties from prototype objects.  The
//  * array returned by this method contains the same values as would
//  * be enumerated by a for-in statement over this object.
//  */
// V8EXPORT Local<Array> GetPropertyNames();
//
// /**
//  * This function has the same functionality as GetPropertyNames but
//  * the returned array doesn't contain the names of properties from
//  * prototype objects.
//  */
// V8EXPORT Local<Array> GetOwnPropertyNames();
//
// /**
//  * Get the prototype object.  This does not skip objects marked to
//  * be skipped by __proto__ and it does not consult the security
//  * handler.
//  */
// V8EXPORT Local<Value> GetPrototype();
//
// /**
//  * Set the prototype object.  This does not skip objects marked to
//  * be skipped by __proto__ and it does not consult the security
//  * handler.
//  */
// V8EXPORT bool SetPrototype(Handle<Value> prototype);
//
// /**
//  * Finds an instance of the given function template in the prototype
//  * chain.
//  */
// V8EXPORT Local<Object> FindInstanceInPrototypeChain(
//     Handle<FunctionTemplate> tmpl);
//
// /**
//  * Call builtin Object.prototype.toString on this object.
//  * This is different from Value::ToString() that may call
//  * user-defined toString function. This one does not.
//  */
// V8EXPORT Local<String> ObjectProtoToString();
//
// /**
//  * Returns the name of the function invoked as a constructor for this object.
//  */
// V8EXPORT Local<String> GetConstructorName();
//
// /** Gets the number of internal fields for this Object. */
// V8EXPORT int InternalFieldCount();
// /** Gets the value in an internal field. */
// inline Local<Value> GetInternalField(int index);
// /** Sets the value in an internal field. */
// V8EXPORT void SetInternalField(int index, Handle<Value> value);
//
// /** Gets a native pointer from an internal field. */
// inline void* GetPointerFromInternalField(int index);
//
// /** Sets a native pointer in an internal field. */
// V8EXPORT void SetPointerInInternalField(int index, void* value);
//
// // Testers for local properties.
// V8EXPORT bool HasOwnProperty(Handle<String> key);
// V8EXPORT bool HasRealNamedProperty(Handle<String> key);
// V8EXPORT bool HasRealIndexedProperty(uint32_t index);
// V8EXPORT bool HasRealNamedCallbackProperty(Handle<String> key);
//
// /**
//  * If result.IsEmpty() no real property was located in the prototype chain.
//  * This means interceptors in the prototype chain are not called.
//  */
// V8EXPORT Local<Value> GetRealNamedPropertyInPrototypeChain(
//     Handle<String> key);
//
// /**
//  * If result.IsEmpty() no real property was located on the object or
//  * in the prototype chain.
//  * This means interceptors in the prototype chain are not called.
//  */
// V8EXPORT Local<Value> GetRealNamedProperty(Handle<String> key);
//
// /** Tests for a named lookup interceptor.*/
// V8EXPORT bool HasNamedLookupInterceptor();
//
// /** Tests for an index lookup interceptor.*/
// V8EXPORT bool HasIndexedLookupInterceptor();
//
// /**
//  * Turns on access check on the object if the object is an instance of
//  * a template that has access check callbacks. If an object has no
//  * access check info, the object cannot be accessed by anyone.
//  */
// V8EXPORT void TurnOnAccessCheck();
//
// /**
//  * Returns the identity hash for this object. The current implementation
//  * uses a hidden property on the object to store the identity hash.
//  *
//  * The return value will never be 0. Also, it is not guaranteed to be
//  * unique.
//  */
// V8EXPORT int GetIdentityHash();
//
// /**
//  * Access hidden properties on JavaScript objects. These properties are
//  * hidden from the executing JavaScript and only accessible through the V8
//  * C++ API. Hidden properties introduced by V8 internally (for example the
//  * identity hash) are prefixed with "v8::".
//  */
// V8EXPORT bool SetHiddenValue(Handle<String> key, Handle<Value> value);
// V8EXPORT Local<Value> GetHiddenValue(Handle<String> key);
// V8EXPORT bool DeleteHiddenValue(Handle<String> key);
//
// /**
//  * Returns true if this is an instance of an api function (one
//  * created from a function created from a function template) and has
//  * been modified since it was created.  Note that this method is
//  * conservative and may return true for objects that haven't actually
//  * been modified.
//  */
// V8EXPORT bool IsDirty();
//
// /**
//  * Clone this object with a fast but shallow copy.  Values will point
//  * to the same values as the original object.
//  */
// V8EXPORT Local<Object> Clone();
//
// /**
//  * Returns the context in which the object was created.
//  */
// V8EXPORT Local<Context> CreationContext();
//
// /**
//  * Set the backing store of the indexed properties to be managed by the
//  * embedding layer. Access to the indexed properties will follow the rules
//  * spelled out in CanvasPixelArray.
//  * Note: The embedding program still owns the data and needs to ensure that
//  *       the backing store is preserved while V8 has a reference.
//  */
// V8EXPORT void SetIndexedPropertiesToPixelData(uint8_t* data, int length);
// V8EXPORT bool HasIndexedPropertiesInPixelData();
// V8EXPORT uint8_t* GetIndexedPropertiesPixelData();
// V8EXPORT int GetIndexedPropertiesPixelDataLength();
//
// /**
//  * Set the backing store of the indexed properties to be managed by the
//  * embedding layer. Access to the indexed properties will follow the rules
//  * spelled out for the CanvasArray subtypes in the WebGL specification.
//  * Note: The embedding program still owns the data and needs to ensure that
//  *       the backing store is preserved while V8 has a reference.
//  */
// V8EXPORT void SetIndexedPropertiesToExternalArrayData(
//     void* data,
//     ExternalArrayType array_type,
//     int number_of_elements);
// V8EXPORT bool HasIndexedPropertiesInExternalArrayData();
// V8EXPORT void* GetIndexedPropertiesExternalArrayData();
// V8EXPORT ExternalArrayType GetIndexedPropertiesExternalArrayDataType();
// V8EXPORT int GetIndexedPropertiesExternalArrayDataLength();
//
// /**
//  * Checks whether a callback is set by the
//  * ObjectTemplate::SetCallAsFunctionHandler method.
//  * When an Object is callable this method returns true.
//  */
// V8EXPORT bool IsCallable();
//
// /**
//  * Call an Object as a function if a callback is set by the
//  * ObjectTemplate::SetCallAsFunctionHandler method.
//  */
// V8EXPORT Local<Value> CallAsFunction(Handle<Object> recv,
//                                      int argc,
//                                      Handle<Value> argv[]);
//
// /**
//  * Call an Object as a constructor if a callback is set by the
//  * ObjectTemplate::SetCallAsFunctionHandler method.
//  * Note: This method behaves like the Function::NewInstance method.
//  */
// V8EXPORT Local<Value> CallAsConstructor(int argc,
//                                         Handle<Value> argv[]);

Object::operator VALUE() {
  if (handle->IsFunction()) {
    // return Function(handle);
  }
  if (handle->IsArray()) {
    // return Array(handle);
  }
  if (handle->IsDate()) {
    // return Date(handle);
  }
  if (handle->IsBooleanObject()) {
    // return BooleanObject(handle);
  }
  if (handle->IsNumberObject()) {
    // return NumberObject(handle);
  }
  if (handle->IsStringObject()) {
    // return StringObject(handle);
  }
  if (handle->IsRegExp()) {
    // return RegExp(handle);
  }
  return Ref<v8::Object>::operator VALUE();
}
}