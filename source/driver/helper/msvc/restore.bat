@echo off

call bin\impl.bat "%VSSDK140Install%" restore
call bin\impl.bat "%VSSDK140Install%" restore amd64
call bin\impl.bat "%VSSDK140Install%" restore amd64_x86
call bin\impl.bat "%VSSDK140Install%" restore x86_amd64

pause
