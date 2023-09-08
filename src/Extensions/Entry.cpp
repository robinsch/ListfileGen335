#include <Windows.h>
#include <string>
#include <unordered_set>
#include <fstream>

#include "Detours/detours.h"

static std::unordered_set<std::string> s_files;
static std::ofstream s_listfile = std::ofstream("(listfile)", std::ofstream::out | std::ofstream::trunc);

static int (__stdcall* sub_424B50_orig)(void* handle, char const* path_, int a3, int a4) = (decltype(sub_424B50_orig))0x00424B50;
static int __stdcall sub_424B50_hk(void* handle, char const* path_, int a3, int a4)
{
    int ret = sub_424B50_orig(handle, path_, a3, a4);

    if (ret)
    {
        std::string path(path_);
        auto it = s_files.find(path);
        if (it == s_files.end())
        {
            s_files.insert(path);
            s_listfile << path << '\n';
        }
    }

    return ret;
}

int __stdcall DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DetourTransactionBegin();
        DetourAttach(&(LPVOID&)sub_424B50_orig, sub_424B50_hk);
        DetourTransactionCommit();
    }

    return 1;
}
