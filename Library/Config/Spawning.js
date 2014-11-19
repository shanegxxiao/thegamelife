
exports.execute = function(command, parameters)
{
    var spawn = require('child_process').spawn;
    var iconv = require('iconv-lite');
    free = spawn(command, [parameters]); 

    // 捕获标准输出并将其打印到控制台 
    free.stdout.on('data', function (data) { 
    console.log('标准输出：\n' + iconv.decode(data, 'GBK')); 
    }); 

    // 捕获标准错误输出并将其打印到控制台 
    free.stderr.on('data', function (data) { 
    console.log('标准错误输出：\n' + iconv.decode(data, 'GBK')); 
    }); 

    // 注册子进程关闭事件 
    free.on('exit', function (code, signal) { 
    console.log('子进程已退出，代码：' + code); 
    }); 
};