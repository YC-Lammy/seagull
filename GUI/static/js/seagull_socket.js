const socket = new WebSocket('ws://localhost:5780') 

socket.addEventListener('message', function (event) {
    console.log('Message from server ', event.data);
});
