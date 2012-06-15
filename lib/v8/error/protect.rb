class V8::Error
  module Protect
    def protect
      yield
    rescue Football => e
      e.kickoff!
    rescue Exception => e
      e.extend Football
      e.kickoff!
    end
  end

  module Football
    def kickoff!
      error = V8::C::Exception::Error(message)
      error.SetHiddenValue("rr::Football", V8::C::External::New(self))
      V8::C::ThrowException(error)
    end
  end
end