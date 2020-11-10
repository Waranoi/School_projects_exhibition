@echo off
rem Run clean command by running the clean.bat file or running ".\clean.bat" or ".\make.bat clean" in a console
call source\Window\MinGW\set_distro_paths.bat
call source\Window\MinGW\bin\make.exe clean
pause