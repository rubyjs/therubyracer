require 'v8'

c = V8::Context.new
puts c.eval("18 + 8")
