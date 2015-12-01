var http = require("http");
var url = require("url");

function start(route, handle) 
{
    function onRequest(request, response)
    {
        var postData = "";
        var pathname = url.parse(request.url).pathname;

        console.log("Request for " + pathname + " received.");

        request.setEncoding("utf8");

        request.addListener("data", function(postDataChunk) {
            // called when a new chunk of data is received
            postData += postDataChunk;
            console.log("Received POST data chunk '" +
                        postDataChunk + "'.");
        });
        request.addListener("end", function(chunk) {
            // called when all chunks of data have been received
            route(handle, pathname, response, postData);
        });
    }

    http.createServer(onRequest).listen(8888);

    console.log("Server has started");
}

exports.start = start;

