require 'rubygems'

UPSTREAM  = "ext/v8/upstream"
SCONS     = "#{UPSTREAM}/scons"
V8_SRC      = "#{UPSTREAM}/2.0.6"

manifest = Rake::FileList.new("**/*")
manifest.exclude "ext/**/test/*", "ext/**/test/*", "ext/**/samples/*", "ext/**/benchmarks/*", "#{UPSTREAM}/build", "tmp", "tmp/**/*", "**/*.gem"
Gem::Specification.new do |gemspec|
  $gemspec = gemspec
  gemspec.name = gemspec.rubyforge_project = "therubyracer"
  gemspec.version = "0.5.5"
  gemspec.summary = "Embed the V8 Javascript interpreter into Ruby"
  gemspec.description = "Call javascript code and manipulate javascript objects from ruby. Call ruby code and manipulate ruby objects from javascript."
  gemspec.email = "cowboyd@thefrontside.net"
  gemspec.homepage = "http://github.com/cowboyd/therubyracer"
  gemspec.authors = ["Charles Lowell", "Bill Robertson"]
  gemspec.extra_rdoc_files = ["README.rdoc"]
  gemspec.executables = ["therubyracer", "v8"]
  gemspec.extensions = ["ext/v8/extconf.rb"]
  gemspec.require_paths = ["lib", "ext"]
  gemspec.files = manifest.to_a
end

begin
  gem 'rake-compiler', '>= 0.4.1'
  require "rake/extensiontask"

  Rake::ExtensionTask.new("v8", $gemspec) do |ext|    
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
task :spec => :compile

desc "Build gem"
task :gem do
  Gem::Builder.new($gemspec).build
end

desc "build the gemspec"
task :gemspec do
  File.open("#{$gemspec.name}.gemspec", "w") do |f|
    f.write($gemspec.to_ruby)
  end
end

task :clean do
  sh "rm -f ext/v8/Makefile"
  sh "rm -rf pkg"
end

namespace :clean do
  desc "remove all built v8 objects"
  task "v8" => "clean" do
    sh "cd #{UPSTREAM} && make clean"
  end  
end


