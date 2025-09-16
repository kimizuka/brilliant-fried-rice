import DMX from 'dmx';
import admin from 'firebase-admin';
import fs from 'fs';

import express from 'express';
import http from 'http';
import path from 'path';
import { Server as SocketIOServer } from 'socket.io';


const dev = '/dev/tty.usbserial-EN437503';
const dmx = new DMX();
const universe = dmx.addUniverse('dmx', 'enttec-usb-dmx-pro', dev);
const serviceAccount = JSON.parse(fs.readFileSync(path.resolve('./brilliant-fried-rice-firebase-adminsdk-fbsvc-8f0bd8d7be.json'), 'utf8'));

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

let blinkTimer = -1;

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount)
});

const db = admin.firestore();
const docRef = db.collection('brilliant-fried-rice').doc('state');
const unsubscribe = docRef.onSnapshot(
  (docSnapshot) => {
    if (docSnapshot.exists) {
      try {
        const { isOpen } = docSnapshot.data();

        io.emit('isOpen', isOpen);

        if (isOpen) {
          startBlink();
        } else {
          stopBlink();
        }
        console.log(docSnapshot.data());
      } catch (error) {
        console.error(error);
      }
    }
  },
  (error) => {
    console.error(error);
  }
);

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
    }, 500);
  }, 1000);
}

function stopBlink() {
  clearInterval(blinkTimer);
  universe.update({ 2: 0 });
}
