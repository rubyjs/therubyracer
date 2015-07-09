#include "rr.h"

namespace rr {
    VALUE ScriptOrigin::Class;
  void ScriptOrigin::Init() {
    ClassBuilder("ScriptOrigin").
      defineSingletonMethod("new", &initialize).

      defineMethod("ResourceName", &ResourceName).
      defineMethod("ResourceLineOffset", &ResourceLineOffset).
      defineMethod("ResourceColumnOffset", &ResourceColumnOffset).
      defineMethod("ScriptID", &ScriptID).
      defineMethod("SourceMapUrl", &SourceMapUrl).

      store(&Class);
  }

  VALUE ScriptOrigin::initialize(int argc, VALUE argv[], VALUE self) {
    Container* container = new Container();
    rb_scan_args(argc, argv, "17",
                 &container->name,
                 &container->line_offset,
                 &container->column_offset,
                 &container->is_shared_cross_origin,
                 &container->script_id,
                 &container->is_embedder_debug_script,
                 &container->source_map_url,
                 &container->is_opaque);
    return ScriptOrigin(container);
  }

  VALUE ScriptOrigin::ResourceName(VALUE self) {
    return ScriptOrigin(self).container->name;
  }

  VALUE ScriptOrigin::ResourceLineOffset(VALUE self) {
    return ScriptOrigin(self).container->line_offset;
  }

  VALUE ScriptOrigin::ResourceColumnOffset(VALUE self) {
    return ScriptOrigin(self).container->column_offset;
  }

  VALUE ScriptOrigin::ScriptID(VALUE self) {
    return ScriptOrigin(self).container->script_id;
  }

  VALUE ScriptOrigin::SourceMapUrl(VALUE self) {
    return ScriptOrigin(self).container->source_map_url;
  }
}
