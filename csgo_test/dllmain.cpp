// dllmain.cpp : Определяет точку входа для приложения DLL.
#define WIN32_LEAN_AND_MEAN 
#define _CRT_SECURE_NO_WARNINGS 
#pragma warning(disable : 4996) 
#include "pch.h"
#include <iostream>
#include <tlhelp32.h>
#include <string>
#include <vector>
#include "minhook/minhook.h"






void initedhook()
{
    if (MH_Initialize() != MH_OK)
    {
        return;
    }


  
    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        std::cout << "Failed to enable hook" << std::endl;
        return;
    }

    printf("successful hooked! \n");
}



typedef void(__stdcall* devMsg)();
devMsg msg;
typedef void(__stdcall* ConsoleMess)(const char*);
ConsoleMess mess;
typedef void(__stdcall* runscript)();
runscript script;
void servermessege()
{
    auto server = (void(__thiscall**)(int*, void***, const char*, signed int))(0x1081C4C8 + 12);
    std::printf("comm -> %s", server);
}


DWORD m_unAccountID = 0x9DC;
DWORD m_bDamaged = 0x0B0D;
DWORD m_iName = 0x154;
DWORD m_iFOV = 0x31F4;

void MainThread(HMODULE modulet)
{
    while (!GetModuleHandleA("serverbrowser.dll"))
        Sleep(100);
    uintptr_t internlmodule = (uintptr_t)(GetModuleHandle(NULL));
    uintptr_t interntiero = (uintptr_t)(GetModuleHandleA("tier0.dll"));
    uintptr_t moduleServer = (uintptr_t)(GetModuleHandleA("server.dll"));
    std::printf("module server %d \n", moduleServer);
    uintptr_t moduleEngine = (uintptr_t)(GetModuleHandleA("engine.dll"));
    uintptr_t client = (uintptr_t)(GetModuleHandleA("client.dll"));
    std::printf("module engine %d \n", moduleEngine);
    uintptr_t panorama = (uintptr_t)(GetModuleHandleA("panorama.dll"));
    std::printf("panorama module -> %d \n", panorama);
    uintptr_t ui = (uintptr_t)(GetModuleHandleA("panoramauiclient.dll"));
    std::printf("panoramauiclient module -> %d \n", ui);
    uintptr_t mc = (uintptr_t)(GetModuleHandleA("matchmaking.dll"));
    std::printf("matchmaking module -> %d \n", mc);

    initedhook();
    //.text:549D66F0 tier0.dll:$66F0 #5AF0 <?DevMsg@@YAXPBDZZ>
    msg = (devMsg)(internlmodule + 0x66F0);
    msg();
   // .text:549D68F0 tier0.dll : $68F0 #5CF0 < ?ConMsg@@YAXPBDZZ>
    mess = (ConsoleMess)(interntiero + 0x68F0);
    mess("test console print");
    //.text:549D9C40 tier0.dll:$9C40 #9040 <?LoggingSystem_Log@@YA?AW4LoggingResponse_t@@HW4LoggingSeverity_t@@VColor@@PBDZZ>
  //  servermessege();
    //.text:546E5B80 tier0.dll:$5B80 #4F80 <?EnterScope@CVProfile@@QAEXPBDH0_NH@Z>
    auto accId = *reinterpret_cast<DWORD*>(client + m_unAccountID);
    std::printf("account id -> %s", accId);
 
    
    

    script = (runscript)(interntiero + 0x5B80);
    script();
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        AllocConsole();
        freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
        freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0,0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

