source 'http://rubygems.org'

gemspec

gem 'redjs', :git => 'git://github.com/cowboyd/redjs.git'
gem "rake"
gem "rspec", "~> 2.0"
gem "rake-compiler"
gem 'gem-compiler' unless Object.const_defined?(:RUBY_ENGINE) && RUBY_ENGINE == 'rbx'

group :libv8 do
  gem 'libv8', "~> 3.11.8"
end



