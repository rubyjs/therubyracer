//needed to get around 
(function(noop){
  window = {
    setInterval: noop,
    clearInterval: noop,
    setTimeout: function(callback, delay) {
      callback()
    },
    clearTimeout: noop
  }
})(function() {})