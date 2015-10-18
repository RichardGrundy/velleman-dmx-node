var vdmxAddon = require('./build/Release/vdmx')

console.log('Calling dmxOpen: ',           vdmxAddon.dmxOpen())
console.log('Calling dmxSetMaxChannels: ', vdmxAddon.dmxSetMaxChannels(3))
console.log('Calling dmxSetValue: ',       vdmxAddon.dmxSetValue(0,  64))
console.log('Calling dmxSetValue: ',       vdmxAddon.dmxSetValue(1, 128))
console.log('Calling dmxSetValue: ',       vdmxAddon.dmxSetValue(2, 255))
console.log('Calling dmxClose: ',          vdmxAddon.dmxClose())
