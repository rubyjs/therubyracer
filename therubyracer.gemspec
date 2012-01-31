# -*- encoding: utf-8 -*-
require File.expand_path('../lib/v8/version', __FILE__)

Gem::Specification.new do |gem|
  gem.authors       = ["Charles Lowell"]
  gem.email         = ["cowboyd@thefrontside.net"]
  gem.description   = %q{TODO: Write a gem description}
  gem.summary       = %q{TODO: Write a gem summary}
  gem.homepage      = ""

  gem.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  gem.files         = `git ls-files`.split("\n")
  gem.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  gem.name          = "therubyracer"
  gem.extensions    = ["ext/v8/extconf.rb"]
  gem.require_paths = ["lib", "ext"]
  gem.version       = V8::VERSION

  gem.add_dependency "libv8", "~> 3.8.9"

  gem.add_development_dependency "rake"
  gem.add_development_dependency "rake-compiler"
  gem.add_development_dependency "rspec", "~> 2.0"
end
