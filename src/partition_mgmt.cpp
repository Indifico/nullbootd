#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <partition_mgmt.hpp>

using namespace std;

// Windows API Calls - Partially AI Generated :// i8win32
string winpartition::wchar_t_to_ansi(wchar_t* wideStr)
{
    int sizeNeeded = WideCharToMultiByte(CP_ACP, 0, wideStr, -1, NULL, 0, NULL, NULL);

    // Allocate buffer and convert
    string ansiStr(sizeNeeded - 1, 0); // -1 because std::string handles null terminator
    WideCharToMultiByte(CP_ACP, 0, wideStr, -1, &ansiStr[0], sizeNeeded, NULL, NULL);

    return ansiStr;
}
wstring winpartition::ansi_to_wide(const string& ansiStr) {
    if (ansiStr.empty()) return L"";

    // 1. Calculate required buffer size
    int sizeNeeded = MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), (int)ansiStr.length(), NULL, 0);

    // 2. Allocate buffer (wstring is convenient)
    wstring wideStr(sizeNeeded, 0);

    // 3. Convert string
    MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), (int)ansiStr.length(), &wideStr[0], sizeNeeded);

    return wideStr;
}

vector<string> winpartition::get_partitions()
{
    vector<string> ids;
    wchar_t volumeName[MAX_PATH];
    HANDLE hFind = FindFirstVolumeW(volumeName, ARRAYSIZE(volumeName));

    if (hFind == INVALID_HANDLE_VALUE) return ids;

    do {
        ids.push_back(wchar_t_to_ansi(volumeName));
    } while (FindNextVolumeW(hFind, volumeName, ARRAYSIZE(volumeName)));

    FindVolumeClose(hFind);

    return ids;
}

bool winpartition::is_efi(string guid)
{
    wstring partition_path = ansi_to_wide(guid);

    wchar_t file_system_name[MAX_PATH + 1] = { 0 };

    GetVolumeInformationW(
        partition_path.c_str(),     // Root path of the volume
        nullptr,                // Optional: Volume name buffer
        0,                      // Volume name size
        nullptr,                // Optional: Serial number
        nullptr,                // Optional: Max component length
        nullptr,                // Optional: File system flags
        file_system_name,         // Buffer for the file system name
        MAX_PATH + 1            // File system name buffer size
    );

   if(wchar_t_to_ansi(file_system_name) == "FAT32")
   {
    return true;
   }
    return false;
}

string winpartition::mount(std::string guid)
{
    filesystem::path current_dir = filesystem::current_path();
    string work_dir = current_dir.string();
    filesystem::create_directories("mnt/boot");

    string mount_point_str = work_dir + "\\mnt\\boot\\";
    LPCSTR mount_point = mount_point_str.c_str();

    //SetVolumeMountPointW(mount_point, ansi_to_wide(guid).c_str());
    if (SetVolumeMountPointA(mount_point, guid.c_str())) {
        cout << "Successfully mounted EFI to " << mount_point_str << endl;
    }
    else {
        cerr << "Mount failed. Error: " << GetLastError() << endl;
    }
   return mount_point_str;
}

void winpartition::unmount(std::string mount_point)
{
    const char* mountptr = mount_point.c_str();
    cout << "Unmount : " << mountptr << endl;
    DeleteVolumeMountPointA(mountptr);
    filesystem::current_path("../../../");
    filesystem::remove_all("./mnt");
}