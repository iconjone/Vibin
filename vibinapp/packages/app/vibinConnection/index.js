//class that handles the connection to the server

class vibinchairControl {
  static socket = null
  static status = false
  static waveInfo = {amp1: 750, amp2: 750, amp3L: 750, amp3R: 750, direction: "up", ampSel: 0, ampFade: 1, amps:[ 'amp3R', 'amp3L', 'amp2', 'amp1']}
  static waveInterval = null
  constructor() {
    this.init()
  }
  send = (message) => {
    //maybe do a check to see if the socket is open & message would work
    console.log("Sending message: " + message)
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
    }

    mute = (mBool) => {
        var m = mBool ? 1 : 0
        this.send({
            type: 'command',
            origin: 'app',
            nId: Math.random().toFixed(2),
            target: 'esp',
            command: 'mute',
            value: m,
        })
    // this.send({type: "command", origin: "app", nId: Math.random(), target:"ampm"});// data:{command: "volume", value: vol} });
  }

  //wave function to increment the volume of the amps in a wave pattern between the 3 amps from 750 to 1000
  wave = (update) => {
    this.waveInfo = {amp1: 750, amp2: 750, amp3L: 750, amp3R: 750, direction: 'up', ampSel: 0, ampFade: 1, amps:[ 'amp3R', 'amp3L', 'amp2', 'amp1']}
    this.controlVol('amp1', "L", this.waveInfo.amp1)
    this.controlVol('amp1', "R", this.waveInfo.amp1)
    this.controlVol('amp2', "L", this.waveInfo.amp2)
    this.controlVol('amp2', "R", this.waveInfo.amp2)
    this.controlVol('amp3', "L", this.waveInfo.amp3L)
    this.controlVol('amp3', "R", this.waveInfo.amp3R)
    setInterval(() => {
     if(( this.waveInfo[this.waveInfo.amps[this.waveInfo.ampFade]] >= 1000 && this.waveInfo.direction == "up") || (this.waveInfo[this.waveInfo.amps[this.waveInfo.ampFade]] <= 750 && this.waveInfo.direction == "down" )){
      if(this.waveInfo.direction == "up"){
        this.waveInfo.ampFade++;
        this.waveInfo.ampSel++;
        if(this.waveInfo.ampFade > 3){
          this.waveInfo.direction = "down";
          this.waveInfo.ampFade = 2;
          this.waveInfo.ampSel = 3;
        }
      }else{
        this.waveInfo.ampFade--;
        this.waveInfo.ampSel--;
        if(this.waveInfo.ampFade < 0){
          this.waveInfo.direction = "up";
          this.waveInfo.ampFade = 1;
          this.waveInfo.ampSel = 0;
        }
      }
     }
      this.waveInfo[this.waveInfo.amps[this.waveInfo.ampFade]] += 50;
      this.waveInfo[this.waveInfo.amps[this.waveInfo.ampSel]] += -50;
      var ampFadeName = this.waveInfo.amps[this.waveInfo.ampFade];
      var ampSelName = this.waveInfo.amps[this.waveInfo.ampSel];

      console.log(ampFadeName, ampSelName)

      if(ampFadeName == "amp3L" || ampFadeName == "amp3R"){
        this.controlVol('amp3', ampFadeName.substring(ampFadeName.length - 1), this.waveInfo[ampFadeName])
      }else{
        this.controlVol(ampFadeName, "L", this.waveInfo[ampFadeName])
        this.controlVol(ampFadeName, "R", this.waveInfo[ampFadeName])
      }

      if(ampSelName == "amp3L" || ampSelName == "amp3R"){
        this.controlVol('amp3', ampSelName.substring(ampSelName.length - 1), this.waveInfo[ampSelName])
      }else{
        this.controlVol(ampSelName, "L", this.waveInfo[ampSelName])
        this.controlVol(ampSelName, "R", this.waveInfo[ampSelName])
      }
    }, update) 
  }


  cancelWave = () => {
    clearInterval(waveInterval)
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
