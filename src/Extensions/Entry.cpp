#include <Windows.h>
#include <string>
#include <unordered_set>
#include <fstream>

#include "Detours/detours.h"

static std::unordered_set<std::string> s_files;
static std::ofstream s_listfile = std::ofstream("(listfile)", std::ofstream::out | std::ofstream::trunc);

static char* (__cdecl* sub_428540_orig)(char const* path_, int a2) = (decltype(sub_428540_orig))0x00428540;
static char* __cdecl sub_428540_hk(char const* path, int a2)
{
    auto it = s_files.find(path);
    if (it == s_files.end())
    {
        s_files.insert(path);
        s_listfile << path << '\n';
    }

    return sub_428540_orig(path, a2);
}

int __stdcall DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DetourTransactionBegin();
        DetourAttach(&(LPVOID&)sub_428540_orig, sub_428540_hk);
        DetourTransactionCommit();
    }

    return 1;
}
