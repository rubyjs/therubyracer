require 'rubygems'

UPSTREAM  = "ext/v8/upstream"

manifest = Rake::FileList.new("**/*")
manifest.exclude "lib/v8/*.bundle", "lib/v8/*.so", "ext/**/test/*", "ext/**/test/*", "ext/**/samples/*", "ext/**/benchmarks/*", "#{UPSTREAM}/build", "tmp", "tmp/**/*", "**/*.gem"
Gem::Specification.new do |gemspec|
  $gemspec = gemspec
  gemspec.name = gemspec.rubyforge_project = "therubyracer"
  gemspec.version = "0.7.6.pre"
  gemspec.summary = "Embed the V8 Javascript interpreter into Ruby"
  gemspec.description = "Call javascript code and manipulate javascript objects from ruby. Call ruby code and manipulate ruby objects from javascript."
  gemspec.email = "cowboyd@thefrontside.net"
  gemspec.homepage = "http://github.com/cowboyd/therubyracer"
  gemspec.authors = ["Charles Lowell", "Bill Robertson"]
  gemspec.extra_rdoc_files = ["README.rdoc"]
  gemspec.executables = ["therubyracer", "v8"]
  gemspec.extensions = ["ext/v8/extconf.rb"]
  gemspec.require_paths = ["lib", "ext", "contrib"]
  gemspec.files = manifest.to_a
end

task :default => :spec

desc "Build gem"
task :gem => :gemspec do
  Gem::Builder.new($gemspec).build
end

desc "build the gemspec"
task :gemspec => :clean do
  File.open("#{$gemspec.name}.gemspec", "w") do |f|
    f.write($gemspec.to_ruby)
  end
end

desc "remove all generated artifacts except built v8 objects"
task :clean do
  sh "rm -rf *.gem"
  sh "rm -rf ext/v8/Makefile"
  sh "rm -rf ext/v8/*.bundle ext/v8/*.so"
  sh "rm -rf lib/v8/*.bundle lib/v8/*.so"
end

desc "remove all built v8 objects"
task "v8:clean" => "clean" do
  sh "cd #{UPSTREAM} && make clean"
end

desc "build v8 with debugging symbols (much slower)"
task "v8:debug" do
  sh "cd #{UPSTREAM} && make debug"
end

for file in Dir['tasks/*.rake']
  load file
end


