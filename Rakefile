#!/usr/bin/env rake
require 'bundler/setup'
require "bundler/gem_tasks"

task :clean do
  sh "rm -rf lib/v8/init.bundle lib/v8/init.so"
  sh "rm -rf pkg"
end

GEMSPEC = eval(File.read("therubyracer.gemspec"))

require File.expand_path('../lib/v8/helper', __FILE__)
if V8::mingw?
  GEMSPEC.files += ["vendor/v8.dll"]
end

require "rake/extensiontask"
Rake::ExtensionTask.new("init", GEMSPEC) do |ext|
  ext.ext_dir = "ext/v8"
  ext.lib_dir = "lib/v8"
  ext.source_pattern = "*.{cc,h}"
end

if V8::mingw?
  task "v8:copy" do
    abort "unable to find v8.dll" unless V8::v8_path
    rm_rf "vendor"
    mkdir "vendor"
    cp File.join(V8::v8_path, "v8.dll"), "vendor"
  end
  [:compile, :native].each do |task|
    Rake::Task[task].prerequisites.unshift "v8:copy"
  end
end

require 'rspec/core/rake_task'
RSpec::Core::RakeTask.new(:spec) do |task|
  task.rspec_opts = '--tag ~memory --tag ~threads'
end

task :sanity => [:clean, :compile] do
  sh %q{ruby -Ilib -e "require 'v8'"}
end

NativeGem = "therubyracer-#{V8::VERSION}-#{Gem::Platform.new(RUBY_PLATFORM)}.gem"

task :gem # dummy task

desc "Build #{NativeGem} into the pkg directory"
task "build:native" => [:native, :gem]

desc "Build and install #{NativeGem} into system gems"
task "install:native" => "build:native" do
  sh "gem install pkg/#{NativeGem}"
end

task :default => :spec

