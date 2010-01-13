# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{therubyracer}
  s.version = "0.4.4"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Charles Lowell", "Bill Robertson"]
  s.date = %q{2010-01-14}
  s.description = %q{Embed the V8 Javascript interpreter into Ruby.}
  s.email = ["cowboyd@thefrontside.net", "billrobertson42@gmail.com"]
  s.extensions = ["ext/v8/extconf.rb"]
  s.extra_rdoc_files = ["History.txt", "Manifest.txt", "docs/data_conversion.txt"]
  s.files = ["Doxyfile", "History.txt", "Manifest.txt", "README.rdoc", "Rakefile", "docs/data_conversion.txt", "ext/v8/callbacks.cpp", "ext/v8/callbacks.h", "ext/v8/convert_ruby.cpp", "ext/v8/convert_ruby.h", "ext/v8/convert_string.cpp", "ext/v8/convert_string.h", "ext/v8/convert_v8.cpp", "ext/v8/convert_v8.h", "ext/v8/converters.cpp", "ext/v8/converters.h", "ext/v8/extconf.rb", "ext/v8/v8.cpp", "ext/v8/v8_cxt.cpp", "ext/v8/v8_cxt.h", "ext/v8/v8_func.cpp", "ext/v8/v8_func.h", "ext/v8/v8_msg.cpp", "ext/v8/v8_msg.h", "ext/v8/v8_obj.cpp", "ext/v8/v8_obj.h", "ext/v8/v8_ref.cpp", "ext/v8/v8_ref.h", "ext/v8/v8_script.cpp", "ext/v8/v8_script.h", "ext/v8/v8_standalone.cpp", "ext/v8/v8_standalone.h", "ext/v8/v8_str.cpp", "ext/v8/v8_str.h", "ext/v8/v8_template.cpp", "ext/v8/v8_template.h", "lib/v8.rb", "lib/v8/context.rb", "lib/v8/object.rb", "lib/v8/to.rb", "script/console", "script/destroy", "script/generate", "spec/ext/cxt_spec.rb", "spec/ext/obj_spec.rb", "spec/redjs/README.txt", "spec/redjs/jsapi_spec.rb", "spec/redjs_helper.rb", "spec/spec.opts", "spec/spec_helper.rb", "tasks/rspec.rake", "therubyracer.gemspec"]
  s.homepage = %q{http://github.com/cowboyd/therubyracer}
  s.rdoc_options = ["--main", "README.rdoc"]
  s.require_paths = ["lib", "ext"]
  s.rubyforge_project = %q{therubyracer}
  s.rubygems_version = %q{1.3.5}
  s.summary = %q{Embed the V8 Javascript interpreter into Ruby.}

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<hoe>, [">= 2.3.3"])
    else
      s.add_dependency(%q<hoe>, [">= 2.3.3"])
    end
  else
    s.add_dependency(%q<hoe>, [">= 2.3.3"])
  end
end
