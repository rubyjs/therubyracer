
module V8
  class Portal
    class Templates

      attr_reader :portal

      def initialize(portal)
        @portal = portal
        @constructors = {}
        @methods = {}
        @procs = {}
        @releases = {}
      end

      def to_constructor(ruby_class)
        class_id = ruby_class.object_id
        if constructor = @constructors[class_id]
          return constructor
        else
          constructor = @constructors[class_id] = ConstructorAdapter.new(self, class_id)
          ObjectSpace.define_finalizer(ruby_class, release(@constructors, class_id))
          return constructor
        end
      end

      def to_function(code)
        case code
        when Method, UnboundMethod
          if fn = @methods[code.to_s]
            return fn
          else
            function = @methods[code.to_s] = FunctionAdapter.new(@portal, code)
            #TODO: test this weak behavior
            function.template.MakeWeak(0, release(@methods, code.to_s))
            return function
          end
        else
          if fn = @procs[code]
            return fn
          else
            function = @procs[code] = FunctionAdapter.new(@portal, code)
            #TODO: test this weak behavior
            function.template.MakeWeak(0, release(@procs, code))
            return function
          end
        end
      end

      def proxies
        @portal.proxies
      end

      def release(refs, id)
        release = Release.new(@releases, refs, id)
        @releases[release] = true
        return release
      end

      class Release
        def initialize(releases, refs, id)
          @releases, @refs, @id = releases, refs, id
        end

        def call(*args)
          @refs.delete(@id)
          @releases.delete(self)
        end

      end
    end
  end
end