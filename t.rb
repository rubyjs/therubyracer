require 'v8'

e = V8::Context.new
e.print("Hello World")
e.print("Hello World")
e.print("Hello World")

e2 = V8::Context.new
e2.print("You Suck!")
e2.print("You Suck!")
e2.print("You RULE!")




