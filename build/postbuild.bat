if not exist DISTRO mkdir DISTRO

copy x64\Release\dll_vs2013.dll DISTRO
copy x64\Release\dll_vs2013.lib DISTRO

copy ..\src\mocapData.* DISTRO
copy ..\src\naturalPointConnector.h DISTRO
copy ..\src\testConnector.h DISTRO
copy ..\src\connector.h DISTRO
copy ..\src\atomList.* DISTRO
copy ..\src\decoder.* DISTRO
copy ..\src\encoder.* DISTRO
copy ..\src\defines.h DISTRO
copy ..\src\dataStream.h DISTRO

copy C:\cpp\api\natnet2.7\lib\x64\NatNetLib.dll DISTRO