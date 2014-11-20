
exports.execute = function(command, parameters)
{
    var spawn = require('child_process').spawn;
    var iconv = require('iconv-lite');
    free = spawn(command, parameters); 

    free.stdout.on('data', function (data) { 
    console.log('stdout：\n' + iconv.decode(data, 'GBK')); 
    }); 

    free.stderr.on('data', function (data) { 
    console.log('stderr：\n' + iconv.decode(data, 'GBK')); 
    }); 

    free.on('exit', function (code, signal) { 
    console.log('exit：' + code); 
    }); 
};