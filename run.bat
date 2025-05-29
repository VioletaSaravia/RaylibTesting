@echo off

set WIN32_DEBUGFLAGS=/DDEBUG /MP /W1 /Zi /EHsc /nologo

build.bat win32 debug
cl src\main_debug.cpp /std:c++20 %WIN32_DEBUGFLAGS% /Fobuild\\debug\\ /Fdbuild\\debug\\ /Febuild\\debug\\ /link /OUT:build\\debug\\main_debug.exe