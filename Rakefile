#!/usr/bin/env rake
require "bundler/gem_tasks"

require "rake/extensiontask"

# desc "remove all generated artifacts except built v8 objects"
task :clean do
  sh "rm -rf lib/v8/vm.bundle lib/v8/vm.so"
  sh "rm -rf pkg"
end

Rake::ExtensionTask.new("vm", eval(File.read("therubyracer.gemspec"))) do |ext|
  ext.lib_dir = "lib/v8"
  ext.source_pattern = "*.{cc,h}"
end


require "rspec/core/rake_task"
RSpec::Core::RakeTask.new(:spec)
task :default => :spec



