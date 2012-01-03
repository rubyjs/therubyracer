require 'optparse'
require 'ostruct'

module V8
  module CLI
    def self.run(exename = 'v8', args = [])      
      options = OpenStruct.new
      options.libs = []
      options.libdirs = []
      parser = OptionParser.new
      parser.banner = "Usage: #{exename} [options]"
      parser.on("-r", "--require FILE", "load and execute FILE as JavaScript source") {|r| options.libs << r}
      parser.on("-i", "--interactive", "interactive mode") {options.interactive = true}
      parser.on("-e", "--execute JAVASCRIPT", String, "evaluate JAVASCRIPT and exit") {|src| options.execute = src}
      parser.on("--selftest", "check that therubyracer is functioning properly") {options.selftest = true}
      parser.on_tail("-v", "--version", "Show version and exit") {options.version_info = true}
      parser.on_tail("-h", "--help", "Show this message") do
        puts parser
        exit
      end
      begin
        parser.parse!(args.dup)
      rescue OptionParser::InvalidOption => e
        puts "#{exename}: #{e.message}"
        exit(1)
      end
      if options.version_info
        require 'libv8'
        puts "The Ruby Racer #{V8::VERSION}"
        puts "V8 Version #{Libv8::V8_VERSION}"
        exit
      elsif options.selftest
        self.test        
      end
      Context.new(:with => Shell.new) do |ctx|
        for libfile in options.libs do
          load(ctx,libfile)
        end        
        if options.interactive
          repl(ctx, exename)
        elsif options.execute
          ctx.eval(options.execute, '<execute>')
        else
          begin
            ctx.eval($stdin, '<stdin>')
          rescue Interrupt => e
            puts; exit
          end
        end
      end
    end

    def self.load(ctx, libfile)
      begin
        ctx.load(libfile)
      rescue V8::JSError => e
        puts e.message
        puts e.backtrace(:javascript)
      rescue StandardError => e
        puts e
      end
    end
    
    def self.test
      begin
        require 'rubygems'
        require 'rspec'
        ARGV.clear
        ARGV << File.dirname(__FILE__) + '/../../spec/'
        ::RSpec::Core::Runner.autorun
        exit(0)
      rescue LoadError => e
        puts "selftest requires rspec to be installed (gem install rspec)"
        exit(1)
      end
      
    end

    def self.repl(ctx, exename)
      require 'readline'
      puts "help() for help. quit() to quit."
      puts "The Ruby Racer #{V8::VERSION}"
      puts "Vroom Vroom!"      
      trap("SIGINT") do
        puts "^C"
      end        
      loop do
        line = Readline.readline("#{exename}> ", true)
        begin
          result = ctx.eval(line, '<shell>')
          puts(result) unless result.nil?                
        rescue V8::JSError => e
          puts e.message
          puts e.backtrace(:javascript)
        rescue StandardError => e
          puts e
          puts e.backtrace.join("\n")
        end
      end          
    end            
    
    class Shell
      def to_s
        "[object Shell]"
      end

      def print(string)
        puts string
      end

      def exit(status = 0)
        Kernel.exit(status)
      end
          
      alias_method :quit, :exit
      
      def help(*args)
        <<-HELP
    print(msg)
      print msg to STDOUT    
        
    exit(status = 0)
      exit the shell
      also: quit()
          
    evalrb(source)
      evaluate some ruby source
    HELP
      end
    end
  end
end