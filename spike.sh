#!/usr/bin/env bash

make clean all
ruby -e "require 'v8'; V8::N::Context.new"