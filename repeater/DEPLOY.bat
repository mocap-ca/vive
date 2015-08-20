@SET BUILDVER=10

@SET BUILD=..\releasebuild\release\MocapSocket.exe
@SET OUTDIR=Deployment
@SET QTDIR=C:\cpp\api\Qt5\5.5\msvc2013_64\bin
@SET VICONDIR=C:\cpp\api\viconDataStream\Win64\CPP
@SET NPDIR=C:\cpp\api\natnet\lib\x64


@IF NOT EXIST %OUTDIR% MKDIR %OUTDIR% 

del %OUTDIR%\*.*

@IF NOT EXIST %OUTDIR%\platforms MKDIR %OUTDIR%\platforms

@IF NOT EXIST %BUILD% ( 
   @ECHO Build not found %BUILD%
   @EXIT /B
   )
   
@IF NOT EXIST %QTDIR% ( 
   @ECHO QT directory not found: %QTDIR%
   @EXIT /B
   )

@IF NOT EXIST %VICONDIR% ( 
   @ECHO VICON directory not found: %VICONDIR%
   @EXIT /B
   )
   
@IF NOT EXIST %NPDIR% ( 
   @ECHO Naturalpoint directory not found: %NPDIR%
   @EXIT /B
   )
  

COPY %BUILD% %OUTDIR%\ViveRepeater_%BUILDVER%.exe 

COPY %QTDIR%\icudt54.dll %OUTDIR%
COPY %QTDIR%\icuin54.dll %OUTDIR%
COPY %QTDIR%\icuuc54.dll %OUTDIR%
COPY %QTDIR%\Qt5Network.dll %OUTDIR%
COPY %QTDIR%\Qt5Gui.dll %OUTDIR%
COPY %QTDIR%\Qt5Core.dll %OUTDIR%
COPY %QTDIR%\Qt5Widgets.dll %OUTDIR%
REM COPY %QTDIR%\libGLESv2.dll %OUTDIR%
REM COPY %QTDIR%\libEGL.dll %OUTDIR%
COPY %QTDIR%\..\plugins\platforms\qwindows.dll %OUTDIR%\platforms\qwindows.dll

COPY %VICONDIR%\*.dll %OUTDIR%
COPY %NPDIR%\*.dll %OUTDIR%

@ECHO Creating Zip


@SET OUTZIP=ViveRepeater_%BUILDVER%.zip
@IF EXIST %OUTZIP% DEL %OUTZIP%de

echo Set fso = CreateObject("Scripting.FileSystemObject") > _zipIt.vbs
echo Set objArgs = WScript.Arguments >> _zipIt.vbs
echo InputFolder = fso.GetAbsolutePathName(objArgs(0)) >> _zipIt.vbs
echo ZipFile = fso.GetAbsolutePathName(objArgs(1)) >> _zipIt.vbs
echo fso.CreateTextFile(ZipFile, True).Write "PK" ^& Chr(5) ^& Chr(6) ^& String(18, vbNullChar) >> _zipIt.vbs
echo Set objShell = CreateObject("Shell.Application") >> _zipIt.vbs
echo Set source = objShell.NameSpace(InputFolder).Items >> _zipIt.vbs
echo objShell.NameSpace(ZipFile).CopyHere(source) >> _zipIt.vbs
echo wScript.Sleep 5000 >> _zipIt.vbs

CScript  _zipIt.vbs  %OUTDIR%  ViveRepeater_%BUILDVER%.zip



@PAUSE
