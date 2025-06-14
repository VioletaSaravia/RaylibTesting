@echo off

set WIN32_DEBUGFLAGS=/DDEBUG /MP /W1 /Zi /EHsc /nologo

set GAME=%1

call build.bat %GAME% win32 debug

cl engine\main_debug.cpp /std:c++20 %WIN32_DEBUGFLAGS% /Fogames\%GAME%\build\debug\ /Fdgames\%GAME%\build\debug\ /Fegames\%GAME%\build\debug\ /link /OUT:games\%GAME%\build\debug\main_debug.exe