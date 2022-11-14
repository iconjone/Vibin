//class that handles the connection to the server

class vibinchairControl {
  static socket = null
  static status = false
  constructor() {
    this.init()
  }
  send = (message) => {
    //maybe do a check to see if the socket is open & message would work
    try {
      this.socket.send(JSON.stringify(message))
    } catch (error) {
      console.log(error)
    }
  }

  controlVol = (amp, channel, vol) => {
    //0L, 0R, 1L, 1R
    this.send({
      type: 'command',
      origin: 'app',
      nId: Math.random().toFixed(2),
      target: amp,
      command: 'volume' + channel,
      value: vol,
    })
  }

  controlMasterVol = (vol) => {
    this.send({
      type: 'command',
      origin: 'app',
      nId: Math.random().toFixed(2),
      target: 'ampm',
      command: 'volume',
      value: vol,
    })
    // this.send({type: "command", origin: "app", nId: Math.random(), target:"ampm"});// data:{command: "volume", value: vol} });
  }
  init = () => {
    this.socket = new WebSocket('ws://vibinchair.local/ws')
    this.status = false
    this.socket.onopen = () => {
      this.status = true
      console.log('connected to server')
    }
    this.socket.onclose = () => {
      this.status = false
      console.log('disconnected from server')
    }
    this.socket.onmessage = (message) => {
      console.log(message)
    }
  }
}

export { vibinchairControl }
