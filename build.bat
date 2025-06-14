@echo off

set GAME=%1
set PLATFORM=%2
set CONFIG=%3

set WIN32_LIBS=vendor\win32\raylib.lib kernel32.lib user32.lib shell32.lib winmm.lib gdi32.lib opengl32.lib msvcrt.lib
set WIN32_DEBUGFLAGS=/DDEBUG /MP /W1 /Zi /EHsc /nologo
set WIN32_RELEASEFLAGS=/W4 /Zi /EHsc /nologo

if /i "%PLATFORM%"=="win32" (
    if /I "%CONFIG%"=="debug" (
        cl games\%GAME%\main.cpp /std:c++20 %WIN32_DEBUGFLAGS% /Ivendor /Fogames\\%GAME%\\build\\debug\\ /Fdgames\\%GAME%\\build\\debug\\ /Fegames\\%GAME%\\build\\debug\\ /link %WIN32_LIBS% /DLL /OUT:games\\%GAME%\\build\\debug\\game.dll /EXPORT:Init /EXPORT:Update /EXPORT:Close /EXPORT:WindowShouldClose
    ) else (
       cl engine\main.cpp /std:c++20 %WIN32_RELEASEFLAGS% /Ivendor /Fogames\\%GAME%\\build\\win32\\ /Fdgames\\%GAME%\\build\\win32\\ /Fegames\\%GAME%\\build\\win32\\ /link %WIN32_LIBS% /OUT:games\\%GAME%\\build\\win32\\main.exe
    )
) else if "%PLATFORM%"=="wasm" (
    echo Not implemented.
)