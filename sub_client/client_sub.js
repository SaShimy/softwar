// subber.js
const zmq = require('zeromq');
const sock = zmq.socket('sub');

sock.connect('tcp://127.0.0.1:4243');
sock.subscribe('test');
console.log('Subscriber connected to port 4243');

sock.on('message', function(topic, message) {
	console.log('received a message related to:', topic.toString(), 'containing message:', message.toString());
});