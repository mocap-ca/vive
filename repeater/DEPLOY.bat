@SET BUILDVER=08

@SET BUILD=..\release\release\MocapSocket.exe
@SET OUTDIR=Deployment
@SET QTDIR=C:\cpp\api\Qt5.3.1\5.3\msvc2010_opengl\bin
@SET VICONDIR=C:\cpp\api\viconDataStream\Win32\CPP

@MKDIR %OUTDIR% 

@IF NOT EXIST %OUTDIR%\platforms MKDIR %OUTDIR%\platforms

@IF EXIST %BUILD% ( COPY %BUILD% %OUTDIR%\ViveRepeater_%BUILDVER%.exe ) ELSE ( @ECHO Could not find build: %CD%/%BUILD% )

@IF EXIST %QTDIR% GOTO QtExists
@ECHO Could not find QT: %QTDIR%
EXIT /B
:QtExists

COPY %QTDIR%\icudt52.dll %OUTDIR%
COPY %QTDIR%\icuin52.dll %OUTDIR%
COPY %QTDIR%\icuuc52.dll %OUTDIR%
COPY %QTDIR%\Qt5Network.dll %OUTDIR%
COPY %QTDIR%\Qt5Gui.dll %OUTDIR%
COPY %QTDIR%\Qt5Core.dll %OUTDIR%
COPY %QTDIR%\Qt5Widgets.dll %OUTDIR%
COPY %QTDIR%\libGLESv2.dll %OUTDIR%
COPY %QTDIR%\libEGL.dll %OUTDIR%
COPY %QTDIR%\..\plugins\platforms\qwindows.dll %OUTDIR%\platforms\qwindows.dll

@REM 	IF NOT DEFINED VICON GOTO SkipVicon

@IF EXIST %VICONDIR% GOTO ViconExists
@ECHO Could not find Vicon: %VICONDIR%
:ViconExists

COPY %VICONDIR%\*.dll %OUTDIR%

:SkipVicon

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
