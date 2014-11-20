fs = require('fs');
rimraf = require('rimraf');
var rd_folders = ['../Boost',
'../OpenSSL',
'../Otl',
'../Vld',
'../Network',
'../Runtime',
'../Database',
'../Utility'];
for (var i = 0; i < rd_folders.length; ++i)
{
    if (fs.existsSync(rd_folders[i]))
    {
        rimraf.sync(rd_folders[i]);
    }
}

var md_folders = ['../Network',
'../Network/Network',
'../Network/Lib',
'../Runtime',
'../Runtime/Runtime',
'../Runtime/Lib',
'../Database',
'../Database/Database',
'../Database/Lib',
'../Utility',
'../Utility/Utility',
'../Utility/Lib'];
for (var i = 0; i < md_folders.length; ++i)
{
    if (!fs.existsSync(md_folders[i]))
    {
        fs.mkdirSync(md_folders[i]);
    }
}

var ex_files = ['../Boost_1_57_0.7z',
'../OpenSSL.7z',
'../Otl.7z',
'../Vld.7z'];
command = require('./Spawning');
for (var i = 0; i < ex_files.length; ++i)
{
    command.execute('7z', ['x', ex_files[i], '-o../']);
}