require 'rubygems'
gem 'hoe', '>= 2.1.0'
require 'hoe'
require 'fileutils'
begin
  require './lib/v8' 
rescue LoadError
  #it will fail to load if we don't have the extensions compiled yet
end

gem 'rake-compiler', '>= 0.4.1'
require "rake/extensiontask"


Hoe.plugin :newgem

# Generate all the Rake tasks
# Run 'rake -T' to see list of generated tasks (from gem root directory)
$hoe = Hoe.spec 'therubyracer' do
  developer 'Charles Lowell', 'cowboyd@thefrontside.net'
  developer 'Bill Robertson', 'billrobertson42@gmail.com'
  self.rubyforge_name = self.name
  self.spec_extras = { :extensions => ["ext/v8/extconf.rb"] }
  self.clean_globs << "lib/v8/*.{o,so,bundle,a,log,dll}"
end

UPSTREAM_SRC  = File.dirname(__FILE__) + "/ext/v8/upstream"
SCONS_SRC     = "#{UPSTREAM_SRC}/scons"
V8_SRC      = "#{UPSTREAM_SRC}/2.0.6"

task "clean-v8" => "clean" do
  sh "rm -f #{V8_SRC}/libv8.a"
  sh "rm -rf #{SCONS_SRC}/build"
  sh "rm -rf #{SCONS_SRC}/install"
  sh "rm -rf #{V8_SRC}/obj"
end


Rake::ExtensionTask.new("v8", $hoe.spec) do |ext|
  ext.lib_dir = "lib/v8"
  ext.source_pattern = "*.{cpp,h}"
end


task :clean do
  sh "rm -f ext/v8/Makefile"
end

require 'newgem/tasks'
Dir['tasks/**/*.rake'].each { |t| load t }

# TODO - want other tests/tasks run by default? Add them to the list
# remove_task :default
# task :default => [:spec, :features]
