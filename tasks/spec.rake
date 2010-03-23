begin
  require 'spec/rake/spectask'
  Spec::Rake::SpecTask.new(:spec) do |spec|
    spec.libs << 'lib' << 'spec'
    spec.spec_files = FileList['spec/**/*_spec.rb']
  end
  task :spec => :compile
rescue LoadError => e
  puts "unable to run specs from rake. gem install rspec"
end
