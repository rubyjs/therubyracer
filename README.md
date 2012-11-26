# therubyracer

* [http://github.com/cowboyd/therubyracer](http://github.com/cowboyd/therubyracer)
* [http://groups.google.com/group/therubyracer](http://groups.google.com/group/therubyracer)
* [irc://irc.freenode.net/therubyracer](http://groups.google.com/group/therubyracer)
* [Documentation](https://github.com/cowboyd/therubyracer/wiki)

## DESCRIPTION

Embed the V8 JavaScript interpreter into Ruby.


## FEATURES

* Evaluate JavaScript from within Ruby
* Embed your Ruby objects into the JavaScript world
* Manipulate JavaScript objects and call JavaScript functions from Ruby
* API compatible with the The Ruby Rhino (for JRuby: http://github.com/cowboyd/therubyrhino)

## SYNOPSIS

    gem install therubyracer

then in your Ruby code

    require 'v8'
    # or if using bundler (as with Rails), add the following to your Gemfile
    gem "therubyracer", :require => 'v8'

evaluate some simple JavaScript

    cxt = V8::Context.new
    cxt.eval('7 * 6') #=> 42

embed values into the scope of your context

    cxt['foo'] = "bar"
    cxt.eval('foo') # => "bar"

embed Ruby code into your scope and call it from JavaScript

    cxt["say"] = lambda {|this, word, times| word * times}
    cxt.eval("say('Hello', 3)") #=> HelloHelloHello

embed a Ruby object into your scope and access its properties/methods from JavaScript

    class MyMath
      def plus(lhs, rhs)
        lhs + rhs
      end
    end

    cxt['math'] = MyMath.new
    cxt.eval("math.plus(20,22)") #=> 42

make a Ruby object *be* your global JavaScript scope.

    math = MyMath.new
    V8::Context.new(:with => math) do |cxt|
      cxt.eval("plus(20,22)") #=> 42
    end

you can do the same thing with Object#eval_js

    math.eval_js("plus(20,22)")

## Different ways of loading JavaScript source

In addition to just evaluating strings, you can also use streams, such as files.

evaluate bytes read from any File/IO object:

    File.open("mysource.js") do |file|
      cxt.eval(file, "mysource.js")
    end

or load it by filename

    cxt.load("mysource.js")


## Safe by default, dangerous by demand

The Ruby Racer is designed to let you evaluate JavaScript as safely as possible unless you tell it to do something more
dangerous. The default context is a hermetically sealed JavaScript environment with only the standard JavaScript objects
and functions. Nothing from the Ruby world is accessible at all.

For Ruby objects that you explicitly embed into JavaScript, by default only the _public_ methods _below_ `Object` are
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

More documentation can be found on the [GitHub wiki](https://github.com/cowboyd/therubyracer/wiki)

## PREREQUISITES

For platforms for which there is a binary version of therubyracer gem available, there are no

dependencies other than Ruby and rubygems.

If there is not a binary version for your system, then you will need to compile it from source.
To do this, you must have v8 >= 3.11.8 installed somewhere on your system. There are several
ways of doing this. For both, you will need a C++ compiler.

The first method involves using a version of the v8 source, which is maintained
[as a RubyGem called libv8][1]. To use it, all you have to do is
add the following to your Gemfile:

    gem 'libv8', '~> 3.11.8'

This will download and build v8 from source for you as part of the gem installation
process. When therubyracer is installed, it will find this gem if it is present and
link against the v8 binaries contained therein.

If you cannot, or do not wish to use the libv8 RubyGem, you can either install libv8
with you operating system's packaging system or you can [build it from source][2]. If
you build from source, be sure to set the library=shared option. Also, if you install
this shared library into a place that is not on your standard lib and include paths, then
you can pass your non-standard locations to therubyracer using the
`--with-v8-include` and `--with-v8-lib` configuration options.


## DEVELOP
    git clone git://github.com/cowboyd/therubyracer.git
    cd therubyracer
    bundle install
    rake compile

## Sponsored by
<a href="http://thefrontside.net">![The Frontside](http://github.com/cowboyd/therubyracer/raw/master/thefrontside.png)</a>

## LICENSE:

(The MIT License)

Copyright (c) 2009,2010,2011,2012 Charles Lowell

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

[1]: https://github.com/cowboyd/libv8
[2]: http://code.google.com/p/v8/wiki/BuildingWithGYP
