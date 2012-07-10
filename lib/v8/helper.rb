module V8
  module_function

  def find_file(name, where)
    where.split(File::PATH_SEPARATOR).each do |path|
    file = File.join(path, name)
    return file if File.exist? file
    end
    false
  end

  def v8_path
    begin
      require 'libv8'
      return Libv8.libv8_source_path
    rescue LoadError
    end

    v8_path = find_file("libv8.a", ENV['PATH'])
    File.dirname v8_path if v8_path
  end

  def darwin?
    RUBY_PLATFORM =~ /darwin/
  end

  def mingw?
    RUBY_PLATFORM =~ /mingw/
  end
end
