rem Run make command by running the make.bat file or running ".\make.bat" in a console
rem Running the make command through the console lets you add your own parameters. Example ".\make.bat clean"
call source\Window\MinGW\set_distro_paths.bat
call source\Window\MinGW\bin\make.exe %*
pause