0dle:
	C:\MinGW\bin\g++.exe -fdiagnostics-color=always -g src/main.cpp -o bin/0dle -Iinclude -Linclude/PDCurses-3.9 -lpdcurses
	IF exist "bin\levels" (rmdir "bin\levels" /s /q)
	xcopy "src\levels" "bin\levels" /e /i /y