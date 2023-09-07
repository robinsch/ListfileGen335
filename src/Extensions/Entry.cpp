#include <Windows.h>
#include <string>
#include <unordered_set>
#include <fstream>

#include "Detours/detours.h"

static std::unordered_set<std::string> s_files;
static size_t s_basePathLength = 0;
static std::ofstream s_listfile = std::ofstream("(listfile)", std::ofstream::out | std::ofstream::trunc);

static char* (__cdecl* sub_433590_orig)(int a1, int a2, char const* fn) = (decltype(sub_433590_orig))0x00433590;
static char* __cdecl sub_433590_hk(int a1, int a2, char const* fn)
{
    std::string path = fn;
    path.erase(0, s_basePathLength);

    auto it = s_files.find(path);
    if (it == s_files.end())
    {
        s_files.insert(path);
        s_listfile << path << '\n';
    }

    return sub_433590_orig(a1, a2, fn);
}

#pragma warning( push )
#pragma warning( disable : 4244)
int __stdcall DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        wchar_t buffer[MAX_PATH + 1];
        GetModuleFileName(NULL, buffer, MAX_PATH + 1);

        std::wstring t(&buffer[0]);
        std::string clientDir(t.begin(), t.end());

        s_basePathLength = clientDir.substr(0, clientDir.find_last_of("\\")).length() + 1;

        DetourTransactionBegin();
        DetourAttach(&(LPVOID&)sub_433590_orig, sub_433590_hk);
        DetourTransactionCommit();
    }

    return 1;
}
#pragma warning( pop )
