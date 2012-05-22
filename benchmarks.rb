require "v8"
require "Benchmark"

TIMES=10
OBJECTS=100


js= <<JS

function createTest()
{
    var test = {};
    test.objects = [];
    test.seed = 49734321;
    test.summ;

    test.random = function()
    {
        // Robert Jenkins' 32 bit integer hash function.
        test.seed = ((test.seed + 0x7ed55d16) + (test.seed << 12))  & 0xffffffff;
        test.seed = ((test.seed ^ 0xc761c23c) ^ (test.seed >>> 19)) & 0xffffffff;
        test.seed = ((test.seed + 0x165667b1) + (test.seed << 5))   & 0xffffffff;
        test.seed = ((test.seed + 0xd3a2646c) ^ (test.seed << 9))   & 0xffffffff;
        test.seed = ((test.seed + 0xfd7046c5) + (test.seed << 3))   & 0xffffffff;
        test.seed = ((test.seed ^ 0xb55a4f09) ^ (test.seed >>> 16)) & 0xffffffff;
        return (test.seed & 0xfffffff) / 0x10000000;
    };

    test.init = function()
    {
        test.objects = [];
        for(var i=0; i<#{OBJECTS}; i++)
        {
            var hash = {};
            for(var j=0; j<10; j++)
            {
                var isString = test.random();
                var key = "str" + test.random();
                var value;
                if(isString < 0.5)
                    value = "str" + test.random();
                else
                    value = test.random();
                hash[key] = value;
            }
            test.objects[i] = hash;
        }
        return test.objects.length;
    }

    test.findSum = function()
    {
        test.summ = 0;
        var length = test.objects.length;
        for(var i=0; i<length; i++)
        {
            var hash = test.objects[i];
            for (var k in hash)
            {
                if (hash.hasOwnProperty(k) && (typeof(hash[k]) == "number"))
                {
                    test.summ += hash[k];
                }
            }
        }
        return test.summ;
    };

    test.finalCheck = function()
    {
        var summ = 0;
        var length = test.objects.length;
        for(var i=0; i<length; i++)
        {
            var hash = test.objects[i];
            for (var k in hash)
            {
                if (hash.hasOwnProperty(k) && (typeof(hash[k]) == "number"))
                {
                    summ += hash[k];
                }
            }
        }
        return summ == -test.summ;
    };

    test.getObject = function(index)
    {
        if(!test.objects[index]) return {};
        return test.objects[index];
    }

    test.setObject = function(index, object)
    {
        test.objects[index] = object;
    }

    return test;
}

JS



def profile
 # RubyProf.profile  do
 Benchmark.realtime do
    yield
   end
end

def get_res result
  #result.threads[0].top_method.total_time
  result.to_f
end



def call_test(suite, name, times = 1)
  cxt = nil
  V8::C::HandleScope() do
    cxt = suite.CreationContext()
    cxt.Enter()
    times.times do
      test = suite.Get(V8::C::String::New(name))
      test.Call(suite, 0, [])
    end
  end
ensure
  cxt.Exit() if cxt
end

#RubyProf.measure_mode = RubyProf::CPU_TIME
puts "init js context..."
cxt = V8::Context.new
cxt.eval(js)

suite = cxt.eval('var test = createTest(); test;')

puts "run init test"
result =profile do
  call_test suite, 'init', TIMES
end

puts "init time: #{get_res(result) / TIMES} sec."

puts "run findSum test"
call_test(suite, 'init')

result =profile do
  call_test suite, 'findSum', TIMES
end

puts "findSum time: #{get_res(result) / TIMES} sec."


puts "run Objects iversion in ruby test"
call_test suite, 'init'

result =profile do
  cxt.native.Enter()
  V8::C::HandleScope() do
    TIMES.times do |j|
      for i in 0..(OBJECTS-1) do
        obj = suite.Get(V8::C::String::New("objects")).Get(i)
        names = obj.GetPropertyNames()
        for k in 0..names.Length() - 1 do
          name = names.Get(k)
          value = obj.Get(name)
          if value.instance_of? Float
            value = value * -1
          end
          suite.Get(V8::C::String::New("objects")).Set(i, obj)
        end
      end
    end
  end
  cxt.native.Exit()
end

puts "Objects time: #{get_res(result) / TIMES} sec."

puts "run finalCheck test"
call_test suite, 'init'

result =profile do
  call_test suite, 'finalCheck', TIMES
end

puts "final check time: #{get_res(result) / TIMES} sec."