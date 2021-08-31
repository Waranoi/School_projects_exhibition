@echo off
call source\Window\MinGW\set_distro_paths.bat
call source\Window\MinGW\bin\make.exe EXE=Subdivision SRCAPP=source/Applications/SubdivisionApp.cpp
call source\Window\MinGW\bin\make.exe EXE=RayIntersection SRCAPP=source/Applications/RayIntersectionApp.cpp
pause