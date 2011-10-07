# -*- encoding: utf-8 -*-
$:.push File.expand_path("../lib", __FILE__)
require "v8/version"
require 'pathname'

Gem::Specification.new do |s|
  s.name = s.rubyforge_project = "therubyracer"
  s.summary = "Embed the V8 Javascript interpreter into Ruby"
  s.version = V8::VERSION
  s.authors = ["Charles Lowell", "Bill Robertson"]
  s.description = "Call javascript code and manipulate javascript objects from ruby. Call ruby code and manipulate ruby objects from javascript."
  s.homepage = "http://github.com/cowboyd/therubyracer"
  s.email = "cowboyd@thefrontside.net"

  root = Pathname(__FILE__).dirname
  s.files = `git ls-files`.split("\n")
  s.files += Dir.chdir(root.join("spec/redjs")) do
    `git ls-files`.split("\n").map {|f| "spec/redjs/#{f}"}
  end
  s.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  s.extensions = ["ext/v8/extconf.rb"]
  s.require_paths = ["lib", "ext"]

  s.add_dependency "libv8", "~> 3.3.10"

  s.add_development_dependency "rake"
  s.add_development_dependency "rspec", "~> 2.0"
  s.add_development_dependency "rake-compiler"
end
