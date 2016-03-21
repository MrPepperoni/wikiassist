var addon = require('bindings')('addon.node')

console.log('Suggestions: ', addon.getSuggestedReading("Bakkecske", 3))
