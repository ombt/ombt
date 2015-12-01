function start()
{
    console.log("Request Handler 'start' was called.");

    function sleep(msecs)
    {
        var startTime = new Date().getTime();
        while (new Date().getTime() < (startTime + msecs)) ;
    }

    sleep(10000);

    return "Hello Start";
}

function upload()
{
    console.log("Request Handler 'upload' was called.");
    return "Hello Upload";
}

exports.start = start;
exports.upload = upload;
