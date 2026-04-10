#include <iostream>
#include <detect_os.hpp>
#include <load_conf.hpp>
#include <partition_mgmt.hpp>
#include <filesystem>
#include <tests.hpp>

using namespace std;

int main()
{
    const OS system_os = check_os();

    /* // Tests
    test::os(system_os);
    test::parse_file("../../../../test/loader.conf");
    */

    if(system_os == OS::windows)
    {
        vector<string> partitions = winpartition::get_partitions();
        string efi_partition_guid;
        for(int i = 0; i < partitions.size(); i++)
        {
            if(winpartition::is_efi(partitions.at(i)))
            {
                // TODO: plan for multiple FAT32 partitons
                cout << "EFI GUID: " << partitions.at(i) << endl;
                efi_partition_guid = partitions.at(i);
                break;
            }
        }

        filesystem::current_path(".."); // cd up out of bin to install folder
        string mount_point = winpartition::mount(efi_partition_guid);

        try
        {
            filesystem::current_path("mnt/boot/loader");
            for(auto const& dir_entry : filesystem::directory_iterator("."))
            {
                cout << dir_entry.path() << endl;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << endl << endl;
            std::cerr << "Nullbootd only works with systemd-boot. Either you don't have systemd-boot" << endl
                << "or your bootloader is misconfigured. Nullbootd has terminated." << endl;
            winpartition::unmount(mount_point);
            cin.get();
            return 1;
        }

        Global_Config conf = Global_Config("loader.conf");

        conf.load();
        conf.update();
        conf.write();

        try
        {
            winpartition::unmount(mount_point);
        }
        catch(const exception& e)
        {
            cerr << e.what() << endl;
        }
        cin.get();
    }
    return 0;
}