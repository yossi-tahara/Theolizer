@echo off

set VS_PATH=
for /f "usebackq delims=" %%a in (`vswhere\vswhere.exe -version 15.0 -property installationPath`) do set VS_PATH=%%~a

rem Visual C++ environment
call "%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat" %1 >NUL 2>&1

set COMPILER_PATH=
for /f "usebackq delims=" %%a in (`where /F cl.exe`) do (
    set COMPILER_PATH=%%~a
    goto break
)
:break
echo %COMPILER_PATH%
