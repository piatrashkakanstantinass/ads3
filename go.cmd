if .%USERDOMAIN% == .DESKTOP-RKG7LUC goto savasPC
Rem MIF kompiuteris
path C:\PROGRA~2\Dev-Cpp\MinGW64\bin\;%PATH%
mingw32-make.exe
goto :toliau

:savasPC
make

:toliau
main.exe
