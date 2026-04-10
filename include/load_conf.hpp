#ifndef LOAD_CONF_H
#define LOAD_CONF_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

class Global_Config
{
    private:
        std::vector<std::string> settings;

        // File Management
        std::string filepath;
        bool path_defined = false;
        bool file_loaded = false;

        // Options
        std::string default;
        unsigned int timeout = 3;
        enum class console_modes { zero, one, two, keep, max, automatic };
        console_modes console_mode = console_modes::keep;
        bool editor = true;
        bool auto_entries = true;
        bool beep = false;
        // purposely omitting some options for simplicity - might add later

        // Options Regex Patterns
        const std::regex default_pattern{"default"};
        const std::regex timeout_pattern{"timeout"};
        const std::regex console_mode_pattern{"console-mode"};
        const std::regex editor_pattern{"editor"};
        const std::regex auto_entries_pattern{"auto-entries"};
        const std::regex beep_pattern{"beep"};
        const std::regex comment_pattern{"#"};

        // Regex Handler T-T
        void handle_regex_setting(std::string target_line);

        // Set Options
        void set_default(std::string value);
        void set_timeout(std::string value);
        void set_console_mode(std::string value);
        void set_editor(std::string value);
        void set_auto_entries(std::string value);
        void set_beep(std::string value);

        // File Helper
        void shared_load_file();

    public:
        // Constructors
        Global_Config();
        Global_Config(std::string pathname);

        // File Handlers
        void set_filepath(std::string pathname);
        void load();
        void load(std::string pathname);
        void unload();
        void write();

        // Test Update
        void update();

        // Print for Testing
        void print();
};

#endif