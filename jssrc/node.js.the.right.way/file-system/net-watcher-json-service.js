
'use strict';

const fs = require('fs');
const net = require('net');
const filename = process.argv[2];
const server = net.createServer(function(connection) {
    console.log('Subscriber connected');

    connection.write(JSON.stringify({
        type: 'watching',
        file: filename
    }) + "\n");

    let watcher = fs.watch(filename, function() {
        connection.write(JSON.stringify({
            type: 'changed',
            file: filename,
            timestamp: Date.now()
        })+ '\n');
    });

    connection.on('close', function() {
        console.log('Subscriber disconnected.');
        watcher.close();
    });
});

if (!filename) {
    throw Error('NO target filename was given.');
}

server.listen(15432, function() {
    console.log('listeneing for subscribers ...');
});

