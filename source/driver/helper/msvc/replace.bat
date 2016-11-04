@echo off

call bin\impl.bat "%VSSDK140Install%" replace
call bin\impl.bat "%VSSDK140Install%" replace amd64
call bin\impl.bat "%VSSDK140Install%" replace amd64_x86
call bin\impl.bat "%VSSDK140Install%" replace x86_amd64

pause
