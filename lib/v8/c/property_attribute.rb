module V8
  module C
    module PropertyAttribute
      None = 0
      ReadOnly = 1 << 0
      DontEnum = 1 << 1
      DontDelete = 1 << 2
    end
    module AccessControl
      DEFAULT = 0
      ALL_CAN_READ = 1 << 0
      ALL_CAN_WRITE = 1 << 1
      PROHIBITS_OVERWRITING = 1 << 2
    end
  end
end
