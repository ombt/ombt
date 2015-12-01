function start()
{
    console.log("Request Handler 'start' was called.");
    return "Hello Start";
}

function upload()
{
    console.log("Request Handler 'upload' was called.");
    return "Hello Upload";
}

exports.start = start;
exports.upload = upload;
