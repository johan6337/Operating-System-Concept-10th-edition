#include <windows.h>
#include <iostream>

bool CopyFileWindowsAPI(const wchar_t* sourceFile, const wchar_t* destFile) {
    // Mở file nguồn với quyền đọc
    HANDLE hSource = CreateFileW(sourceFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSource == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to open source file. Error: " << GetLastError() << std::endl;
        return false;
    }

    // Mở file đích với quyền ghi (tạo file nếu không tồn tại, ghi đè nếu có)
    HANDLE hDest = CreateFileW(destFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDest == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to open destination file. Error: " << GetLastError() << std::endl;
        CloseHandle(hSource);
        return false;
    }

    // Đọc và ghi dữ liệu
    const DWORD bufferSize = 1024; // Kích thước bộ đệm
    char buffer[bufferSize];
    DWORD bytesRead, bytesWritten;

    while (true) {
        // Đọc từ file nguồn
        if (!ReadFile(hSource, buffer, bufferSize, &bytesRead, NULL) || bytesRead == 0) {
            break; // Khi đọc xong hoặc có lỗi
        }

        // Ghi vào file đích
        if (!WriteFile(hDest, buffer, bytesRead, &bytesWritten, NULL) || bytesRead != bytesWritten) {
            std::wcerr << L"Failed to write to destination file. Error: " << GetLastError() << std::endl;
            CloseHandle(hSource);
            CloseHandle(hDest);
            return false;
        }
    }

    // Đóng file
    CloseHandle(hSource);
    CloseHandle(hDest);

    return true;
}

int main() {
    wchar_t sourceFile[MAX_PATH];
    wchar_t destFile[MAX_PATH];

    // Nhập đường dẫn file nguồn từ người dùng
    std::wcout << L"Enter source file path: ";
    std::wcin.getline(sourceFile, MAX_PATH);

    // Nhập đường dẫn file đích từ người dùng
    std::wcout << L"Enter destination file path: ";
    std::wcin.getline(destFile, MAX_PATH);

    if (CopyFileWindowsAPI(sourceFile, destFile)) {
        std::wcout << L"File copied successfully!" << std::endl;
    } else {
        std::wcout << L"File copy failed!" << std::endl;
    }

    return 0;
}
