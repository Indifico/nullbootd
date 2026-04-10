cmake_minimum_required(VERSION 3.10.0)
project(nullbootd VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(nullbootd src/main.cpp src/detect_os.cpp src/load_conf.cpp src/partition_mgmt.cpp src/tests.cpp)
include(InstallRequiredSystemLibraries)


# Standard CPack variables
set(CPACK_PACKAGE_NAME "nullbootd")
set(CPACK_PACKAGE_VENDOR "Indifico")
set(CPACK_PACKAGE_VERSION "0.0.1")

# Set the generator to NSIS
set(CPACK_GENERATOR "NSIS")

# Optional NSIS-specific settings
set(CPACK_NSIS_MODIFY_PATH ON) # Adds an option to add program to PATH
#set(CPACK_NSIS_HELP_LINK "https://example.com") # Support URL

# 1. Define the base root (e.g., C:\)
set(CPACK_NSIS_INSTALL_ROOT "C:")

# 2. Define the application-specific folder
set(CPACK_PACKAGE_INSTALL_DIRECTORY "nullbootd")

# Resulting default path: C:\MyCustomFolder

# Install the redistributable to the installer directory
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/redist/vc_redist.x64.exe" DESTINATION "redist")

# Add a silent install command to the NSIS script
# /install /quiet /norestart are standard silent flags for MSVC redists
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
  "ExecWait '$\\\"$INSTDIR\\\\redist\\\\vc_redist.x64.exe$\\\" /install'" #/quiet /norestart'"
)

target_include_directories(nullbootd PRIVATE include)

set_target_properties(nullbootd PROPERTIES 
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""
)

install(TARGETS nullbootd DESTINATION bin)
install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin)

include(CPack)