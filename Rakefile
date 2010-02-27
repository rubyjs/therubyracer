require 'rubygems'

UPSTREAM  = "ext/v8/upstream"
SCONS     = "#{UPSTREAM}/scons"
V8_SRC      = "#{UPSTREAM}/2.0.6"


begin
  require 'jeweler'
  Jeweler::Tasks.new do |gemspec|
    gemspec.name = gemspec.rubyforge_project = "therubyracer"
    gemspec.version = "0.5.2"
    gemspec.summary = "Embed the V8 Javascript interpreter into Ruby"
    gemspec.description = "Call javascript code and manipulate javascript objects from ruby. Call ruby code and manipulate ruby objects from javascript."
    gemspec.email = "cowboyd@thefrontside.net"
    gemspec.homepage = "http://github.com/cowboyd/therubyracer"
    gemspec.authors = ["Charles Lowell", "Bill Robertson"]
    gemspec.extra_rdoc_files = ["README.rdoc"]
    gemspec.files.exclude "ext/**/test/*", "ext/**/samples/*", "ext/**/benchmarks/*", "#{UPSTREAM}/build"
  end
rescue LoadError
  puts "Jeweler not available. Install it with: gem install jeweler"
end

begin
  gem 'rake-compiler', '>= 0.4.1'
  require "rake/extensiontask"

  Rake::ExtensionTask.new("v8", eval("#{Rake.application.jeweler.gemspec.to_ruby}")) do |ext|    
    ext.lib_dir = "lib/v8"
    ext.source_pattern = "*.{cpp,h}"
  end  
rescue LoadError
  puts "Rake Compiler not available. Install it with: gem install rake-compiler"
end

require 'spec/rake/spectask'
Spec::Rake::SpecTask.new(:spec) do |spec|
  spec.libs << 'lib' << 'spec'
  spec.spec_files = FileList['spec/**/*_spec.rb']
end

desc "Build gem"
task :gem => :build

desc "Do not call this, call gem instead."
task :build


task "clean-v8" => "clean" do
  sh "cd #{UPSTREAM} && make clean"
end

task :clean do
  sh "rm -f ext/v8/Makefile"
  sh "rm -rf pkg"
end
