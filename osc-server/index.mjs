import DMX from 'dmx';
import osc from 'osc';
import player from 'play-sound';

import express from 'express';
import http from 'http';
import path from 'path';
import { Server as SocketIOServer } from 'socket.io';

const dev = '/dev/tty.usbserial-EN437503';
const dmx = new DMX();
const universe = dmx.addUniverse('dmx', 'enttec-usb-dmx-pro', dev);

const udpPort = new osc.UDPPort({
  localAddress: '0.0.0.0',
  localPort: 10000,
  remoteAddress: '192.168.0.3',
  remotePort: 10000,
  broadcast: true,
  metadata: true
});

const app = express();
const server = http.createServer(app);

app.use(express.static(path.join(process.cwd(), 'public')));

const io = new SocketIOServer(server);

io.on('connection', (socket) => {
  console.log('A user connected:', socket.id);

  socket.on('disconnect', () => {
    console.log('User disconnected:', socket.id);
  });
});

const PORT = process.env.PORT || 3000;

server.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
});

let audio = null;
let blinkTimer = -1;

udpPort.on('message', (message) => {
  if (message.address === '/open') {
    startBlink();
    audio = player().play('bgm.mp3', (err) => {
      if (err && !err.killed) throw err
    });
    io.emit('isOpen', true);
  } else if (message.address === '/close') {
    stopBlink();
    audio.kill();
    io.emit('isOpen', false);
  }
});

udpPort.open();

// process.on('SIGINT', () => {
//   unsubscribe();
//   server.close(() => {
//     process.exit(0);
//   });
// });

function startBlink() {
  blinkTimer = setInterval(() => {
    console.log('on');
    universe.update({ 2: 255 });
    setTimeout(() => {
      console.log('off');
      universe.update({ 2: 0 });
    }, 200);
  }, 400);
}

function stopBlink() {
  clearInterval(blinkTimer);
  universe.update({ 2: 0 });
}
