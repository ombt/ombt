
'use strict';

const fs = require('fs');
const net = require('net');
const filename = process.argv[2];
const server = net.createServer(function(connection) {
    console.log('Subscriber connected');
    connection.write("Now watching '" + filename + "' for changes ...\n");

    let watcher = fs.watch(filename, function() {
        connection.write("File '" + filename + "' changed: " + 
                         Date.now() + "\n");
    });

    connection.on('close', function() {
        console.log('Subscriber disconnected.');
        watcher.close();
    });
});

if (!filename) {
    throw Error('NO target filename was given.');
}

server.listen('/tmp/watcher.sock', function() {
    console.log('listening for subscribers ...');
});

