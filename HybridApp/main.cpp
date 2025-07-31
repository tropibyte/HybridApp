#include "pch.h"
#include "HybridApp.h"
#include <tlhelp32.h>

static DWORD GetParentProcessId()
{
    DWORD pid = GetCurrentProcessId();
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE)
        return 0;
    PROCESSENTRY32 pe = { sizeof(pe) };
    DWORD ppid = 0;
    if (Process32First(snap, &pe)) {
        do {
            if (pe.th32ProcessID == pid) {
                ppid = pe.th32ParentProcessID;
                break;
            }
        } while (Process32Next(snap, &pe));
    }
    CloseHandle(snap);
    return ppid;
}

static void SuspendOrResumeProcess(DWORD pid, bool suspend)
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snap == INVALID_HANDLE_VALUE)
        return;
    THREADENTRY32 te = { sizeof(te) };
    if (Thread32First(snap, &te)) {
        do {
            if (te.th32OwnerProcessID == pid) {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
                if (hThread) {
                    if (suspend)
                        SuspendThread(hThread);
                    else
                        ResumeThread(hThread);
                    CloseHandle(hThread);
                }
            }
        } while (Thread32Next(snap, &te));
    }
    CloseHandle(snap);
}

int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    bool forceGui = false;
    bool forceConsole = false;
    for (int i = 1; i < argc; ++i) {
        if (_wcsicmp(argv[i], L"/gui") == 0)
            forceGui = true;
        else if (_wcsicmp(argv[i], L"/console") == 0)
            forceConsole = true;
    }
    bool attached = false;
    DWORD parentPid = GetParentProcessId();
    if (!forceGui) {
        if (AttachConsole(ATTACH_PARENT_PROCESS)) {
            attached = true;
            if (parentPid)
                SuspendOrResumeProcess(parentPid, true);
        } else if (forceConsole) {
            AllocConsole();
            attached = true;
        }
    } else if (forceConsole) {
        AllocConsole();
        attached = true;
    }

    if (attached) {
        FILE* f;
        freopen_s(&f, "CONIN$", "r", stdin);
        freopen_s(&f, "CONOUT$", "w", stdout);
        freopen_s(&f, "CONOUT$", "w", stderr);
    }

    int ret = AfxWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    if (attached && parentPid) {
        SuspendOrResumeProcess(parentPid, false);
    }
    return ret;
}
