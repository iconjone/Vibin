//class that handles the connection to the server

class vibinchairControl {
    static socket = null;
    static status = false;
    constructor(){
        this.socket = new WebSocket('ws://vibinchair.local/ws');
        this.status = false;
        this.socket.onopen = () => {
            this.status = true;
            console.log("connected to server");
        }
        this.socket.onclose = () => {
            this.status = false;
            console.log("disconnected from server");
        }
        this.socket.onmessage = (message) => {
            console.log(message);
        }
    }
    send = (message) => {
        this.socket.send(message);
    }
    
}

export {vibinchairControl};
