require 'bundler'
require 'bundler/setup'
require "rake/extensiontask"
require "rspec/core/rake_task"
Bundler::GemHelper.install_tasks

task :default => :spec

desc "remove all generated artifacts except built v8 objects"
task :clean do
  sh "rm -rf pkg"
  sh "rm -rf ext/v8/Makefile"
  sh "rm -rf ext/v8/*.bundle ext/v8/*.so"
  sh "rm -rf lib/v8/*.bundle lib/v8/*.so"
end

desc "remove all built v8 objects"
task "v8:clean" => "clean" do
  sh "cd ext/v8/upstream && make clean"
end

desc "build v8 with debugging symbols (much slower)"
task "v8:debug" do
  sh "cd ext/v8/upstream && make debug"
end

Rake::ExtensionTask.new("v8", eval(File.read("therubyracer.gemspec"))) do |ext|    
  ext.lib_dir = "lib/v8"
  ext.source_pattern = "*.{cpp,h}"
end

RSpec::Core::RakeTask.new(:spec)


