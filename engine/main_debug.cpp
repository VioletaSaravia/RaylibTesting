#include "Windows.h"
#include <stdio.h>

static struct {
    HMODULE     library;
    const char *path;
    FILETIME    prevWriteTime;
    FILETIME    lastWriteTime;

    INT_PTR(__stdcall *Init)();
    bool (*WindowShouldClose)();
    INT_PTR(__stdcall *Update)();
    INT_PTR(__stdcall *Close)();
} AppDll;

int main() {
    AppDll.path              = "build/debug/game.dll";
    AppDll.library           = LoadLibraryA(AppDll.path);
    AppDll.Init              = GetProcAddress(AppDll.library, "Init");
    AppDll.WindowShouldClose = (bool (*)())GetProcAddress(AppDll.library, "WindowShouldClose");
    AppDll.Update            = GetProcAddress(AppDll.library, "Update");
    AppDll.Close             = GetProcAddress(AppDll.library, "Close");

    auto hFile = CreateFileA(AppDll.path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file (error %lu)\n", GetLastError());
        return 1;
    }

    if (!GetFileTime(hFile, NULL, NULL, &AppDll.prevWriteTime)) {
        printf("Failed to get file time (error %lu)\n", GetLastError());
        CloseHandle(hFile);
        return 1;
    }
    printf("BLA");

    AppDll.Init();
    while (!AppDll.WindowShouldClose()) {
        AppDll.Update();

        if (!GetFileTime(hFile, NULL, NULL, &AppDll.lastWriteTime)) {
            printf("Failed to get file time (error %lu)\n", GetLastError());
            CloseHandle(hFile);
            return 1;
        }

        if (AppDll.prevWriteTime.dwHighDateTime >= AppDll.lastWriteTime.dwHighDateTime) continue;

        AppDll.prevWriteTime = AppDll.lastWriteTime;
    }
    AppDll.Close();

    return 0;
}