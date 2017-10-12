const zmq = require("zeromq");
const requests = [];
const PORT = "4242";

const client4 = zmq.socket("req");
client4.identity = "#0x04";

client4.connect("tcp://127.0.0.1:4242");
client4.send("identify|"+"#0x04")
console.log("Client4 connected to server.");