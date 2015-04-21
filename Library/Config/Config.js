fs = require('fs');
rimraf = require('rimraf');
command = require('./Spawning');

var args = process.argv.splice(2);
var configObj = JSON.parse(fs.readFileSync(args[0]));
var folder2rm = configObj.folder2rm;
var folder2mk = configObj.folder2mk;
var file2extract = configObj.file2extract;

for (var i = 0; i < folder2rm.length; ++i)
{
    if (fs.existsSync(folder2rm[i]))
    {
        rimraf.sync(folder2rm[i]);
    }
}

for (var i = 0; i < folder2mk.length; ++i)
{
    if (!fs.existsSync(folder2mk[i]))
    {
        fs.mkdirSync(folder2mk[i]);
    }
}

for (var i = 0; i < file2extract.length; ++i)
{
    var suffix = file2extract[i].substring(file2extract[i].lastIndexOf('.'), file2extract[i].length).toLowerCase();
    if (suffix === '.7z')
    {
        command.execute('7za', ['x', file2extract[i], '-o../']);
    }
    else if (suffix === '.bz2')
    {
        command.execute('tar', ['zxvf', file2extract[i], '-C../']);
    }
    else
    {
        console.log("ERROR! UNKNOWN FILE FORMAT!");
    }
}