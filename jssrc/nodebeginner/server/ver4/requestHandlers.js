var exec = require("child_process").exec;

function start(response)
{
    console.log("Request Handler 'start' was called.");

    exec("ls -lah", function (error, stdout, stderr) {
        response.writeHead(200, {"Content-Type": "test/plain"});
        response.write(stdout);
        response.end();
    });
}

function find(response)
{
    console.log("Request Handler 'find' was called.");

    exec("find /",
         { timeout: 10000, maxBuffer: 200000*1024 },
         function (error, stdout, stderr) {
             response.writeHead(200, {"Content-Type": "test/plain"});
             response.write(stdout);
             response.end();
         });
}

function upload(response)
{
    console.log("Request Handler 'upload' was called.");
    response.writeHead(200, {"Content-Type": "test/plain"});
    response.write("Hello Upload");
    response.end();
}

exports.start = start;
exports.find = find;
exports.upload = upload;
