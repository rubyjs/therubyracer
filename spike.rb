`make all`
require 'v8';

#manually run a script
result = V8::C::Context.new.open do |cxt|
  v8_str = V8::C::String.new('1 + 1')
  v8_script = V8::C::Script.new(v8_str)
  v8_script.Run()
end

puts "result: #{result}"


#define eval() purely in ruby
class V8::C::Context
  def eval(javascript)
    self.open do
      source = V8::C::String.new(javascript)
      script = V8::C::Script.new(source)
      script.Run()
    end
  end
end

f = V8::C::FunctionTemplate.new do | foo, bar |
   "Hello!! This is ruby code #{foo} #{bar}!!!!"
end

plus = V8::C::FunctionTemplate.new do |a, b|
  a + b
end

o = V8::C::ObjectTemplate.new
o.Set("hello", f)
o.Set("plus_of_awesomeness", plus)
V8::C::Context.new(o).open do |cxt|
  puts "r1: " + cxt.eval('plus_of_awesomeness(8, 3.4)').to_s
  puts "r2: " + cxt.eval('hello("Fred", "Wilma")')
end



