# therubyracer

* [http://github.com/cowboyd/therubyracer](http://github.com/cowboyd/therubyracer)
* [http://groups.google.com/group/therubyracer](http://groups.google.com/group/therubyracer)
* [irc://irc.freenode.net/therubyracer](http://groups.google.com/group/therubyracer)
* [Documentation](https://github.com/cowboyd/therubyracer/wiki)

## DESCRIPTION

Embed the V8 Javascript interpreter into Ruby.


## FEATURES

* Evaluate Javascript from with in Ruby
* Embed your Ruby objects into the Javascript world
* Manipulate JavaScript objects and call JavaScript functions from Ruby
* API compatible with the The Ruby Rhino (for JRuby: http://github.com/cowboyd/therubyrhino)

## SYNOPSIS

    gem install therubyracer         ;: stable
    gem install therubyracer --pre   ;: bleeding edge

then in your ruby code

    require 'v8'

evaluate some simple javascript

    cxt = V8::Context.new
    cxt.eval('7 * 6') #=> 42

embed values into the scope of your context

    cxt['foo'] = "bar"
    cxt.eval('foo') # => "bar"

embed ruby code into your scope and call it from javascript

    cxt["say"] = lambda {|word, times| word * times}
    cxt.eval("say('Hello', 3)") #=> HelloHelloHello

embed a ruby object into your scope and access its properties/methods from javascript

    class MyMath
      def plus(lhs, rhs)
        lhs + rhs
      end
    end

    cxt['math'] = MyMath.new
    cxt.eval("math.plus(20,22)") #=> 42

make a ruby object *be* your global javascript scope.

    math = MyMath.new
    V8::Context.new(:with => math) do |cxt|
      cxt.eval("plus(20,22)") #=> 42
    end

you can do the same thing with Object#eval_js

    math.eval_js("plus(20,22)")

## Different ways of loading javascript source

In addition to just evaluating strings, you can also use streams such as files.

evaluate bytes read from any File/IO object:

    File.open("mysource.js") do |file|
      cxt.eval(file, "mysource.js")
    end

or load it by filename

    cxt.load("mysource.js")


## Safe by default, dangerous by demand

The Ruby Racer is designed to let you evaluate javascript as safely as possible unless you tell it to do something more
dangerous. The default context is a hermetically sealed javascript environment with only the standard javascript objects
and functions. Nothing from the ruby world is accessible at all.

For ruby objects that you explicitly embed into javascript, by default only the _public_ methods _below_ `Object` are
exposed by default. E.g.

    class A
      def a
        "a"
      end

      def to_s
        super
      end
    end

    class B < A
      def b
        "b"
      end
    end


    V8::Context.new do |cxt|
      cxt['a'] = A.new
      cxt['b'] = B.new
      cxt.eval("a.a") # => 'a'
      cxt.eval("b.b") # => 'b'
      cxt.eval("b.a") # => 'a'
      cxt.eval("b.to_s") # => #<B:0x101776be8> (because A explicitly defined it)
      cxt.eval("b.object_id") #=> undefined, object_id is on Object
    end

If needed, you can override the [Ruby Access](https://github.com/cowboyd/therubyracer/blob/master/lib/v8/access.rb)
to allow whatever behavior you'd like

More documentation can be found on the [github wiki](https://github.com/cowboyd/therubyracer/wiki)

## REQUIREMENTS:

* python >= 2.5 (required to compile v8)
* C++ compiler

## Rails/Bundler

To use the ruby racer in rails, or any application using Bundler to manage gems, add the following to your Gemfile

    gem "therubyracer", :require => 'v8'
    gem "therubyracer", "~> 0.8.2.pre" #bleeding edge.

## DEVELOP
    git clone git://github.com/cowboyd/therubyracer.git
    cd therubyracer
    git submodule update --init
    bundle install
    rake compile

## Sponsored by
<a href="http://thefrontside.net">![The Frontside](http://github.com/cowboyd/therubyracer/raw/master/thefrontside.png)</a>

## LICENSE:

(The MIT License)

Copyright (c) 2009,2010,2011 Charles Lowell

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.