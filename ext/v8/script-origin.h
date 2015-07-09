// -*- mode: c++ -*-
#ifndef SCRIPT_ORIGIN_H
#define SCRIPT_ORIGIN_H

namespace rr {
  class ScriptOrigin {
    struct Container {
      inline Container() {}

      Container(VALUE name_,
                VALUE line_offset_,
                VALUE column_offset_,
                VALUE is_shared_cross_origin_,
                VALUE script_id_,
                VALUE is_embedder_debug_script_,
                VALUE source_map_url_,
                VALUE is_opaque_) :
        name(name_),
        line_offset(line_offset_),
        column_offset(column_offset_),
        is_shared_cross_origin(is_shared_cross_origin_),
        script_id(script_id_),
        is_embedder_debug_script(is_embedder_debug_script_),
        source_map_url(source_map_url_),
        is_opaque(is_opaque_) {}


      VALUE name;
      VALUE line_offset;
      VALUE column_offset;
      VALUE is_shared_cross_origin; //option
      VALUE script_id;
      VALUE is_embedder_debug_script; //option
      VALUE source_map_url;
      VALUE is_opaque; //option

    };
    struct Integer : public Equiv {
      Integer(v8::Handle<v8::Integer> value) :
        Equiv(INT2FIX(value->IntegerValue())) {
      }
    };
  public:
    static void Init();

    ScriptOrigin(VALUE value) {
      Data_Get_Struct(value, struct Container, container);
    }
    ScriptOrigin(Container* container_) :
      container(container_) {
    }
    ScriptOrigin(v8::Isolate* isolate, v8::ScriptOrigin origin) :
      ScriptOrigin(new Container(
                     Value(isolate, origin.ResourceName()),
                     Integer(origin.ResourceLineOffset()),
                     Integer(origin.ResourceColumnOffset()),
                     Bool(origin.Options().IsSharedCrossOrigin()),
                     Integer(origin.ScriptID()),
                     Bool(origin.Options().IsEmbedderDebugScript()),
                     Value(isolate, origin.SourceMapUrl()),
                     Bool(origin.Options().IsOpaque()))) {
    }

    static void mark(Container* container) {
      rb_gc_mark(container->name);
      rb_gc_mark(container->line_offset);
      rb_gc_mark(container->column_offset);
      rb_gc_mark(container->script_id);
      rb_gc_mark(container->source_map_url);
      rb_gc_mark(container->is_shared_cross_origin);
      rb_gc_mark(container->is_embedder_debug_script);
      rb_gc_mark(container->is_opaque);
    }

    static VALUE initialize(int argc, VALUE argv[], VALUE self);

    static void deallocate(Container* container) {
      delete container;
    }
    inline operator VALUE() {
      return Data_Wrap_Struct(Class, &mark, &deallocate, container);
    }
    static VALUE ResourceName(VALUE self);
    static VALUE ResourceLineOffset(VALUE self);
    static VALUE ResourceColumnOffset(VALUE self);
    static VALUE ScriptID(VALUE self);
    static VALUE SourceMapUrl(VALUE self);

    static VALUE Class;
  private:
    Container* container;
  };
}

#endif /* SCRIPT_ORIGIN_H */
