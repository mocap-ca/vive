@SET BUILDDIR1=..\build-MocapSocket-Desktop_Qt_5_3_0_MSVC2013_64bit-Release\release\
@SET BUILDDIR2=..\build-MocapSocket-Desktop_Qt_5_3_0_MSVC2013_64bit-Debug\debug\

@SET VICONDIR=C:\cpp\api\viconDataStream\Win64\CPP

COPY %VICONDIR%\boost_python-vc90-mt-1_48.dll %BUILDDIR1%
COPY %VICONDIR%\boost_thread-vc90-mt-1_48.dll %BUILDDIR1%
COPY %VICONDIR%\ViconDataStreamSDK_CPP.dll %BUILDDIR1%


COPY %VICONDIR%\boost_python-vc90-mt-1_48.dll %BUILDDIR2%
COPY %VICONDIR%\boost_thread-vc90-mt-1_48.dll %BUILDDIR2%
COPY %VICONDIR%\ViconDataStreamSDK_CPP.dll %BUILDDIR2%

pause