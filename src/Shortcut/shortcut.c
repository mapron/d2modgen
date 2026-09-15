#ifdef _WIN32
#include <windows.h>
#include <wchar.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    wchar_t basePath[MAX_PATH];
    GetModuleFileNameW(NULL, basePath, MAX_PATH);

    wchar_t* lastSlash = wcsrchr(basePath, L'\\');
    if (lastSlash != NULL) {
        *lastSlash = L'\0';
    } else {
        return 1; // Fallback failure if path is corrupted
    }

    wchar_t targetExe[MAX_PATH];
    swprintf_s(targetExe, MAX_PATH,
#ifdef _DEBUG
               L"%s\\bin\\modgen_uiD.exe",
#else
               L"%s\\bin\\modgen_ui.exe",
#endif

               basePath);

    STARTUPINFOW        si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcessW(
            targetExe, // Path to real executable
            NULL,      // Pass NULL since we aren't handling CLI arguments
            NULL,      // Process security attributes
            NULL,      // Thread security attributes
            FALSE,     // Inherit handles
            0,         // Creation flags
            NULL,      // Use parent's environment block
            basePath,  // Force the working directory to the installation ROOT
            &si,       // Pointer to STARTUPINFO
            &pi))      // Pointer to PROCESS_INFORMATION
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return 0;
    }

    return 1;
}
#else
int main() { return 0;}
#endif
