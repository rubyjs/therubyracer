//needed to get around 
(function(noop){
  window = {
    setInterval: noop,
    clearInterval: noop,
    setTimeout: noop,
    clearTimeout: noop
  }
})(function() {})