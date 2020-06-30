// dllmain.cpp : Define el punto de entrada de la aplicación DLL.
#include "pch.h"
#include<iostream>
#include<Windows.h>

void placeJMP(BYTE* address, DWORD jumpTo, DWORD length)
{
    DWORD oldProtect, newProtect, relativeAddress;
    VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &oldProtect);
    relativeAddress = (DWORD)(jumpTo - (DWORD)address) - 5;
    *address = 0xE9;
    *((DWORD*)(address + 0x1)) = relativeAddress;
    for (DWORD x = 0x5; x < length; x++)
    {
        *(address + x) = 0x90;
    }
    VirtualProtect(address, length, oldProtect, &newProtect);
}

DWORD base = (DWORD)GetModuleHandleA(0);
DWORD libcocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");
DWORD retJMP = base + 0x190BD5;

BOOL y = false;
DWORD oldProtect;
__declspec(naked) void Ok() {
    __asm pushad
    if (y == false) {
        BYTE nops[] = {
            0x90,0x90
        };

        MessageBoxA(NULL, (LPCSTR)"Editor Hack Enabled!", (LPCSTR)"EditorHack Enabled!", MB_OK);

        VirtualProtect((LPVOID)(base + 0x1E4A32), sizeof(nops), PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy((void*)(base + 0x1E4A32),nops, sizeof(nops));
        VirtualProtect((LPVOID)(base + 0x1E4A32), sizeof(nops), oldProtect, NULL );

        y = true;
        
    }
    else {
        BYTE original[] = {
            0x75,0x6C
        };
        MessageBoxA(NULL, (LPCSTR)"Editor Hack Disabled!", (LPCSTR)"EditorHack Disabled!", MB_OK);

        VirtualProtect((LPVOID)(base + 0x1E4A32), sizeof(original), PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy((void*)(base + 0x1E4A32)  , original, sizeof(original));
        VirtualProtect((LPVOID)(base + 0x1E4A32), sizeof(original), oldProtect, NULL);

        y = false;
    }
    __asm popad
    __asm ret 0x4
}
DWORD uno = base + 0x297B10;
DWORD dos = libcocosbase + 0xD3400;
DWORD tres = base + 0x191C30;
DWORD cuatro = base + 0x18EE0;
DWORD cinco = base + 0x2CD6D4;
DWORD test = (DWORD)Ok;



__declspec(naked) void myMidfuncHook() {

    __asm {
        push uno
        call dos
        mov esi, eax
        mov dword ptr ss : [esp] , 0x3F800000
        mov ecx, esi
        mov edx, dword ptr ds : [esi]
        call dword ptr[edx + 0x50]
        push test  
        push ebx
        mov ecx, esi
        call cuatro
        mov ecx, dword ptr ss : [esp + 0x20]
        add esp, 0x08
        mov edx, dword ptr ds : [ecx]
        push eax
        call dword ptr ds : [edx + 0xE0]
        push  cinco
    }

    __asm jmp[retJMP]
}


void mainHack() {
    /* 
    //Tester moment: 
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "Base: " << std::hex << base << std::endl;
    std::cout << "LibCocosBase: " << std::hex << libcocosbase << std::endl;
    */

    placeJMP((BYTE*)base + 0x190BD0, (DWORD)myMidfuncHook, 0x5);


}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)mainHack, hModule, 0, nullptr);
        //MessageBoxA(NULL, (LPCSTR)"Injected", (LPCSTR)"Injected", MB_OK);
    case DLL_THREAD_ATTACH:


    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


