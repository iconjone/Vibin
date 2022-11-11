//class that handles the connection to the server

class vibinchairControl {
    static socket = null;
    static status = false;
    constructor(){
        this.init();
    }
    send = (message) => {
        //maybe do a check to see if the socket is open & message would work
        this.socket.send(JSON.stringify(message));
    }
    init = () => {
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
}

export {vibinchairControl};
