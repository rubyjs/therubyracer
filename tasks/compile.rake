begin
  gem 'rake-compiler', '>= 0.4.1'
  require "rake/extensiontask"
  Rake::ExtensionTask.new("v8", $gemspec) do |ext|    
    ext.lib_dir = "lib/v8"
    ext.source_pattern = "*.{cpp,h}"
  end  
rescue LoadError
  puts "Rake Compiler not available. You won't be able to compile with rake, unless you gem install rake-compiler"
end
 