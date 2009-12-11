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

f = V8::C::FunctionTemplate.new do
  puts "Hello!! This is ruby code!!!!"
end

f2 = V8::C::FunctionTemplate.new

o = V8::C::ObjectTemplate.new
o.Set("hello", f)
o.Set("nello", f2)
V8::C::Context.new(o).open do |cxt|
  puts "r1: " + cxt.eval('nello()')
  puts "r2: " + cxt.eval('hello()')
end;



