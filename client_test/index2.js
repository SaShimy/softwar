const zmq = require("zeromq");
const requests = [];
const PORT = "4242";

const client2 = zmq.socket("req");
client2.identity = "#0x02";

client2.connect("tcp://127.0.0.1:4242");
client2.send("identify|"+"#0x02")
console.log("Client2 connected to server.");