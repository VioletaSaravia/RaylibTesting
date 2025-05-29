@echo off

set PLATFORM=%1
set CONFIG=%2

set WIN32_LIBS=vendor\win32\raylib.lib kernel32.lib user32.lib shell32.lib winmm.lib gdi32.lib opengl32.lib msvcrt.lib
set WIN32_DEBUGFLAGS=/DDEBUG /MP /W1 /Zi /EHsc /nologo
set WIN32_RELEASEFLAGS=/W4 /Zi /EHsc /nologo

if /i "%PLATFORM%"=="win32" (
    if /I "%CONFIG%"=="debug" (
        cl src\game.cpp /std:c++20 %WIN32_DEBUGFLAGS% /Ivendor /Fobuild\\debug\\ /Fdbuild\\debug\\ /Febuild\\debug\\ /link %WIN32_LIBS% /DLL /OUT:build\\debug\\game.dll /EXPORT:Init /EXPORT:Update /EXPORT:Close /EXPORT:WindowShouldClose
    ) else (
       cl src\main.cpp /std:c++20 %WIN32_RELEASEFLAGS% /Ivendor /Fobuild\\win32\\ /Fdbuild\\win32\\ /Febuild\\win32\\ /link %WIN32_LIBS% /OUT:build\\win32\\main.exe
    )
) else if "%PLATFORM%"=="wasm" (
    echo Not implemented.
)