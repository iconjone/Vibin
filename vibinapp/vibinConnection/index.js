//class that handles the connection to the server

class vibinchairControl {
  constructor() {
    this.ws = new WebSocket("ws://vibinchair.local/ws");
    this.status = false;
    this.ws.onopen = () => {
      this.status = true;
      this.ws.send("{type: 'connection', data: 'Hello'}");
    };
    this.ws.onclose = (e) => {
      this.status = false;
      console.log(e);
    };
    this.ws.onerror = (e) => {
      this.status = false;
      console.log(e);
    };
    this.ws.onmessage = (e) => {
      console.log(e);
    };
  }
  getState() {
    return this.status;
  }
  send(message) {
    if (this.status) {
      this.ws.send(message);
    }
  }
  close() {
    this.ws.close();
  }
}

export { vibinchairControl };
