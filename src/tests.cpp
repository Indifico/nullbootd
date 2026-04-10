#include <detect_os.hpp>
#include <iostream>
#include <string>
#include <load_conf.hpp>
#include <tests.hpp>

using namespace std;

void test::os(const OS system_os)
{
    cout << "OS Test: ";
    if(system_os == OS::windows)
    {
        cout << "You are on Windows!" << endl;
    }
    else if(system_os == OS::linux)
    {
        cout << "You are on Linux!" << endl;
    }
    else
    {
        cout << "This program is for Windows/Linux Dual Boot. "
            << "It would require modifications to run on other systems." << endl;
    }
    cout << endl;
}

void test::parse_file(string pathname)
{
    Global_Config settings = Global_Config(pathname);

    cout << "Loading test file: " << endl;
    settings.load();
    settings.update();

    cout << "Writing to test file..." << endl;
    settings.write();

    cout << "Done with parsing test!" << endl;
    cin.ignore();
    cin.get();
}