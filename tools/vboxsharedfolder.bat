@echo off

SET vbox="D:\Program Files\Oracle\VirtualBox\VBoxManage.exe"

setlocal

cd ..\SDK\

%vbox% sharedfolder remove MoreFun  -name H9G
%vbox% sharedfolder add MoreFun --automount -name H9G --hostpath "%CD%" 

endlocal
