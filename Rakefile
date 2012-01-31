#!/usr/bin/env rake
require "bundler/gem_tasks"

task :clean do
  sh "rm -rf lib/v8/vm.bundle lib/v8/vm.so"
  sh "rm -rf pkg"
end

require "rake/extensiontask"
Rake::ExtensionTask.new("vm", eval(File.read("therubyracer.gemspec"))) do |ext|
  ext.ext_dir = "ext/v8"
  ext.lib_dir = "lib/v8"
  ext.source_pattern = "*.{cc,h}"
end

require 'rspec/core/rake_task'
RSpec::Core::RakeTask.new(:spec)

task :sanity => [:clean, :compile] do
  sh %q{ruby -Ilib -e "require 'v8'"}
end

task :default => :spec

