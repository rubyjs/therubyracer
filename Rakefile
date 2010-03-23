require 'rubygems'

UPSTREAM  = "ext/v8/upstream"
SCONS     = "#{UPSTREAM}/scons"
V8_SRC      = "#{UPSTREAM}/2.0.6"

manifest = Rake::FileList.new("**/*")
manifest.exclude "ext/**/test/*", "ext/**/test/*", "ext/**/samples/*", "ext/**/benchmarks/*", "#{UPSTREAM}/build", "tmp", "tmp/**/*", "**/*.gem"
Gem::Specification.new do |gemspec|
  $gemspec = gemspec
  gemspec.name = gemspec.rubyforge_project = "therubyracer"
  gemspec.version = "0.6.0"
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
  sh "rm -rf *.gem"
end

namespace :clean do
  desc "remove all built v8 objects"
  task "v8" => "clean" do
    sh "cd #{UPSTREAM} && make clean"
  end  
end

for file in Dir['tasks/*.rake']
  load file
end


