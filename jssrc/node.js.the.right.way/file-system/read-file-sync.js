const fs = require('fs');
const data = fs.readFileSync(process.argv[2]);
process.stdout.write(data.toString()+"\n");
