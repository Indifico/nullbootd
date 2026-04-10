#include <detect_os.hpp>

OS check_os()
{
    #ifdef _WIN32
        return OS::windows;
    #elif __linux__
        return OS::linux;
    #else
        return OS::other;
    #endif
}