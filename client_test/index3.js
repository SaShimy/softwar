const zmq = require("zeromq");
const requests = [];
const PORT = "4242";

const client3 = zmq.socket("req");
client3.identity = "#0x03";

client3.connect("tcp://127.0.0.1:4242");
client3.send("identify|"+"#0x03")
console.log("Client3 connected to server.");