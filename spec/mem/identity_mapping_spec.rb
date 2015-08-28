require 'spec_helper'

describe "Identity Mapping Memory Usages" do
  it "does not leak memory by virtue of maintaining an identity map" do
    ##
    # This verifies that the identity map which is holds on to the
    # mappings between v8 objects and ruby objects does not leak its
    # entries as originally reported by this ticket:
    # https://github.com/cowboyd/therubyracer/pull/336
    #
    # Without the patch, while the v8 object was not being leaked, the
    # entry in the identity map was. This test injects 5000 objects
    # into the v8 context 20 times over, which will generate 5000
    # unique proxies. All of these proxies should be immediately released.

    require 'memory_profiler'

    def rss
      `ps -eo pid,rss | grep #{Process.pid} | awk '{print $2}'`.to_i
    end

    cxt = V8::Context.new
    Object.new.tap do |o|
      cxt["object-#{o.object_id}"] = o
      cxt["object-#{o.object_id}"] = nil
    end

    # MemoryProfiler has a helper that runs the GC multiple times to make
    #  sure all objects that can be freed are freed.
    MemoryProfiler::Helpers.full_gc

    start = rss
    #puts "rss: #{rss} live objects #{GC.stat[:heap_live_slots]}"

    20.times do

      5000.times { |i|
        Object.new.tap do |o|
          cxt["object-#{o.object_id}"] = o
          cxt["object-#{o.object_id}"] = nil
        end
        while cxt.isolate.native.IdleNotificationDeadline(0.1); end
      }
      MemoryProfiler::Helpers.full_gc
      #puts "rss: #{rss} live objects #{GC.stat[:heap_live_slots]}"

    end
    finish = rss

    expect(finish).to be <= start * 2.5
  end if RUBY_VERSION >= "2.1.0"
end
