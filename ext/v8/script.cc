#include "rr.h"

namespace rr {

void Script::Init() {
  ClassBuilder("Script").
    defineSingletonMethod("New", &New).
    defineMethod("Run", &Run).
    store(&Class);
  ClassBuilder("ScriptOrigin").
    defineSingletonMethod("new", &ScriptOrigin::initialize).
    store(&ScriptOrigin::Class);
  ClassBuilder("ScriptData").
    defineSingletonMethod("PreCompile", &ScriptData::PreCompile).
    defineSingletonMethod("New", &ScriptData::New).
    defineMethod("Length", &ScriptData::Length).
    defineMethod("Data", &ScriptData::Data).
    defineMethod("HasError", &ScriptData::HasError).
    store(&ScriptData::Class);
}

VALUE ScriptOrigin::initialize(int argc, VALUE argv[], VALUE self) {
  VALUE name; VALUE line_offset; VALUE column_offset;
  rb_scan_args(argc, argv, "12", &name, &line_offset, &column_offset);
  v8::Handle<v8::Integer> loff = v8::Integer::New(RTEST(line_offset) ? NUM2INT(line_offset) : 0);
  v8::Handle<v8::Integer> coff = v8::Integer::New(RTEST(column_offset) ? NUM2INT(column_offset) : 0);
  return ScriptOrigin(new v8::ScriptOrigin(*String(name), loff, coff));
}

VALUE ScriptData::PreCompile(VALUE self, VALUE input, VALUE length) {
#ifdef HAVE_RUBY_ENCODING_H
  if (!rb_equal(rb_enc_from_encoding(rb_utf8_encoding()), rb_obj_encoding(input))) {
    rb_warn("ScriptData::Precompile only accepts UTF-8 encoded source, not: %s", RSTRING_PTR(rb_inspect(rb_obj_encoding(input))));
  }
#endif
  return ScriptData(v8::ScriptData::PreCompile(RSTRING_PTR(input), NUM2INT(length)));
}
VALUE ScriptData::New(VALUE self, VALUE data, VALUE length) {
  return ScriptData(v8::ScriptData::New(RSTRING_PTR(data), NUM2INT(length)));
}
VALUE ScriptData::Length(VALUE self) {
  return ScriptData(self)->Length();
}
VALUE ScriptData::Data(VALUE self) {
  ScriptData data(self);
#ifdef HAVE_RUBY_ENCODING_H
  return rb_enc_str_new(data->Data(), data->Length(), rb_enc_find("BINARY"));
#else
  return rb_str_new(data->Data(), data->Length());
#endif
}

VALUE ScriptData::HasError(VALUE self) {
  return ScriptData(self)->HasError();
}

VALUE Script::New(int argc, VALUE argv[], VALUE self) {
  VALUE source; VALUE origin; VALUE pre_data; VALUE script_data;
  rb_scan_args(argc, argv, "13", &source, &origin, &pre_data, &script_data);
  if (argc == 2) {
    VALUE filename = origin;
    return Script(v8::Script::New(String(source), Value(filename)));
  } else {
    return Script(v8::Script::New(String(source), ScriptOrigin(origin), ScriptData(pre_data), String(script_data)));
  }
}

VALUE Script::Run(VALUE self) {
  return Value(Script(self)->Run());
}

template <> void Pointer<v8::ScriptData>::unwrap(VALUE value) {
  Data_Get_Struct(value, class v8::ScriptData, pointer);
}

template <> void Pointer<v8::ScriptOrigin>::unwrap(VALUE value) {
  Data_Get_Struct(value, class v8::ScriptOrigin, pointer);
}

} //namespace rr