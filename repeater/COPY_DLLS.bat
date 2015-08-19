@SET BUILDDIR1=..\releasebuild\release\
@SET BUILDDIR2=..\debugbuild\debug\

@SET VICONDIR=C:\cpp\api\viconDataStream\Win64\CPP

COPY %VICONDIR%\boost_python-vc90-mt-1_48.dll %BUILDDIR1%
COPY %VICONDIR%\boost_thread-vc90-mt-1_48.dll %BUILDDIR1%
COPY %VICONDIR%\ViconDataStreamSDK_CPP.dll %BUILDDIR1%


COPY %VICONDIR%\boost_python-vc90-mt-1_48.dll %BUILDDIR2%
COPY %VICONDIR%\boost_thread-vc90-mt-1_48.dll %BUILDDIR2%
COPY %VICONDIR%\ViconDataStreamSDK_CPP.dll %BUILDDIR2%

pause