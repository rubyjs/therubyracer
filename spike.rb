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

cxt = V8::C::Context.new
my_result = cxt.eval('7 * 6')
puts "my result: #{my_result}"



