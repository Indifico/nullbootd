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

    // Find /loader folder in EFI partition (OS specific)
    string mount_point;
    if(system_os == OS::windows)
    {
        mount_point = winpartition::find_and_mount_efi();
    }

    Global_Config conf = Global_Config("loader.conf"); // Create config object with pathname
    conf.load();

    // TODO: Make update function interactive
    conf.update();

    conf.write();

    // Unmount partition when done
    if(system_os == OS::windows)
    {
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