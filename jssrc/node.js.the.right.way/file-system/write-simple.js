const fs = require('fs');
fs.writeFile('target.txt', "I'm done, Mr. Duck", function(err,data) {
    if (err) {
        throw err;
    }
    console.log("File saved");
});
