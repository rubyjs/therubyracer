module V8
  module C
    # Shim to support the old style locking syntax. We don't currently
    # deprecate this because it might make a comeback at some point.
    #
    # to synchronize access to V8, and to associate V8 with this thread:
    #
    #     Locker() do
    #       #... interact with v8
    #     end
    def self.Locker
      lock = Locker.new
      yield
    ensure
      lock.delete
    end
  end
end