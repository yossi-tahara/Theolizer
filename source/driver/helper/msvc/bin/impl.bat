setlocal

set OPE_MODE=%2
set BUILD_MODE=%3
set COMPILER="%~1..\VC\bin\%BUILD_MODE%\cl.exe"

if exist %COMPILER% (
    call "%~1..\VC\vcvarsall.bat" %BUILD_MODE%
    bin\TheolizerDriver.exe --theolizer-%OPE_MODE%=%COMPILER%
)

endlocal
