module V8
  module C
    # Shim to support the old style locking syntax. We don't currently
    # deprecate this because it might make a comeback at some point.
    def Locker
      lock = Locker.new
      yield
    ensure
      lock.delete
    end
  end
end