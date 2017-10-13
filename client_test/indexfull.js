const zmq = require("zeromq");
const requests = [];
const PORT = "4242";

const client = zmq.socket("req");
client.identity = "#0x01";

client.connect("tcp://127.0.0.1:4242");
client.send("identify|"+"#0x01")
console.log("Client1 connected to server.");

const client2 = zmq.socket("req");
client2.identity = "#0x02";

client2.connect("tcp://127.0.0.1:4242");
client2.send("identify|"+"#0x02")
console.log("Client2 connected to server.");

const client3 = zmq.socket("req");
client3.identity = "#0x03";

client3.connect("tcp://127.0.0.1:4242");
client3.send("identify|"+"#0x03")
console.log("Client3 connected to server.");


const client4 = zmq.socket("req");
client4.identity = "#0x04";

client4.connect("tcp://127.0.0.1:4242");
client4.send("identify|"+"#0x04")
console.log("Client4 connected to server.");