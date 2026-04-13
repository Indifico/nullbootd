#ifndef PARTITION_MGMT_H
#define PARTITION_MGMT_H

namespace winpartition
{
    namespace internal // Helper functions
    {
        std::string wchar_t_to_ansi(wchar_t* wideStr);
        std::wstring ansi_to_wide(const std::string& ansiStr);
        std::vector<std::string> get_partitions();
        bool is_efi(std::string guid);
        std::string mount(std::string guid);
    }

    std::string find_and_mount_efi();
    void unmount(std::string mount_point);
}

namespace linuxpartition
{

}

#endif