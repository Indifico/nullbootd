#include <load_conf.hpp>

using namespace std;

// Private
void Global_Config::handle_regex_setting(string target_line)
{
    if (!regex_search(target_line, comment_pattern)) // if the line hasn't been commented out, check the line
    {
        // remove the pattern from the line string and set value
        if (regex_search(target_line, default_pattern))
        {
            target_line = regex_replace(target_line, default_pattern, "");
            stringstream string_stream(target_line);
            string_stream >> target_line;
            set_default(target_line);
        }
        else if (regex_search(target_line, timeout_pattern))
        {
            target_line = regex_replace(target_line, timeout_pattern, "");
            stringstream string_stream(target_line);
            string_stream >> target_line;
            set_timeout(target_line);
        }
        else if (regex_search(target_line, console_mode_pattern))
        {
            target_line = regex_replace(target_line, console_mode_pattern, "");
            stringstream string_stream(target_line);
            string_stream >> target_line;
            set_console_mode(target_line);
        }
        else if (regex_search(target_line, editor_pattern))
        {
            target_line = regex_replace(target_line, editor_pattern, "");
            stringstream string_stream(target_line);
            string_stream >> target_line;
            set_editor(target_line);
        }
        else if (regex_search(target_line, auto_entries_pattern))
        {
            target_line = regex_replace(target_line, auto_entries_pattern, "");
            stringstream string_stream(target_line);
            string_stream >> target_line;
            set_auto_entries(target_line);
        }
        else if (regex_search(target_line, beep_pattern))
        {
            target_line = regex_replace(target_line, beep_pattern, "");
            stringstream string_stream(target_line);
            string_stream >> target_line;
            set_beep(target_line);
        }
    }
}
// Private - Setting Helpers
void Global_Config::set_default(string value)
{
    default = value;
}
void Global_Config::set_timeout(string value)
{
    try //stoi reads the string no matter what is passed
    {
        if(stoi(value) < 0)
        {
            throw exception("Value cannot be negative!");
        }
        timeout = stoi(value);
    }
    catch(const exception& e)
    {
        cerr << "Timeout value is not a positive integer! " << "Exception: " << e.what() << endl;
        cerr << "Given Value: " << value << endl;
        cerr << "Please input a new positive integer value for timeout to continue: ";
        cin >> value;
        set_timeout(value);
    }
}
void Global_Config::set_console_mode(string value)
{
    try
    {
        int numerical_value = stoi(value);
        if(numerical_value >= 0 && numerical_value <= 2)
        {
            if(numerical_value == 0)
            {
                console_mode = console_modes::zero;
            }
            else if(numerical_value == 1)
            {
                console_mode = console_modes::one;
            }
            else if(numerical_value == 2)
            {
                console_mode = console_modes::two;
            }
        }
        else
        {
            throw runtime_error("Value is numeric but not 0-2!");
        }
    }
    catch(const exception& e)
    {
        try
        {
            if(value == "auto")
            {
                console_mode = console_modes::automatic;
            }
            else if(value == "max")
            {
                console_mode = console_modes::max;
            }
            else if(value == "keep")
            {
                console_mode = console_modes::keep;
            }
            else
            {
                throw runtime_error("Value is neither 0-2 nor a valid literal option!");
            }
        }
        catch(const exception& f)
        {
            cerr << "console_mode was not a valid option!" << endl
                << "Exceptions: " << e.what() << " " << f.what() << endl
                << "Please enter a valid option" << endl
                << "(0, 1, 2, auto, max, keep): ";
            cin >> value;
            set_console_mode(value);
        }
    }
}
void Global_Config::set_editor(string value)
{
    try
    {
        if(value == "yes" || value == "y" || value == "true" || value == "t" || value == "on" || value == "1")
        {
            editor = true;
        }
        else if(value == "no" || value == "n" || value == "false" || value == "f" || value == "off" || value == "0")
        {
            editor = false;
        }
        else
        {
            throw runtime_error("Invalid editor value found!");
        }
    }
    catch(const runtime_error& e)
    {
        cerr << e.what() << endl
            << "Continuting with default (true)" << endl;
        editor = true;
    }
}
void Global_Config::set_auto_entries(string value)
{
    try
    {
        if(value == "yes" || value == "y" || value == "true" || value == "t" || value == "on" || value == "1")
        {
            auto_entries = true;
        }
        else if(value == "no" || value == "n" || value == "false" || value == "f" || value == "off" || value == "0")
        {
            auto_entries = false;
        }
        else
        {
            throw runtime_error("Invalid auto-entries value found!");
        }
    }
    catch(const runtime_error& e)
    {
        cerr << e.what() << endl
            << "Continuting with default (true)" << endl;
        auto_entries = true;
    }
}
void Global_Config::set_beep(string value)
{
    try
    {
        if(value == "yes" || value == "y" || value == "true" || value == "t" || value == "on" || value == "1")
        {
            beep = true;
        }
        else if(value == "no" || value == "n" || value == "false" || value == "f" || value == "off" || value == "0")
        {
            beep = false;
        }
        else
        {
            throw runtime_error("Invalid beep value found!");
        }
    }
    catch(const runtime_error& e)
    {
        cerr << e.what() << endl
            << "Continuting with default (false)" << endl;
        beep = false;
    }
}
// Private - File Helper
void Global_Config::shared_load_file()
{
    try
    {
        if(path_defined)
        {
            ifstream conf_file(filepath);
            if(conf_file.is_open())
            {
                string current_line;
                while(getline(conf_file, current_line))
                {
                    handle_regex_setting(current_line);
                }
                file_loaded = true;
                conf_file.close();
            }
            else
            {
                throw runtime_error("File failed to load!");
            }
        }
        else
        {
            throw runtime_error("Filepath is underfined!");
        }
    }
    catch(const runtime_error& e)
    {
        cerr << "Error: " << e.what() << endl
            << "Please try a different path: ";
        string pathname;
        cin >> pathname;
        set_filepath(pathname);
        shared_load_file();
    }
    cout << endl;
    print();
    cout << endl;
}
// Public
Global_Config::Global_Config()
{

}
Global_Config::Global_Config(string pathname)
{
    filepath = pathname;
    path_defined = true;
}
void Global_Config::set_filepath(string pathname)
{
    filepath = pathname;
    path_defined = true;
}
void Global_Config::load()
{
    shared_load_file();
}
void Global_Config::load(string pathname)
{
    set_filepath(pathname);
    shared_load_file();
}
void Global_Config::unload()
{
    settings.clear();
    file_loaded = false;
}
void Global_Config::write()
{
    ofstream conf_out("loader.conf");
    try
    {
        if(conf_out.is_open())
        {
            conf_out << "default " << default << endl
                << "timeout " << timeout << endl
                << "console-mode ";

            if(static_cast<int>(console_mode) <= 2)
            {
                conf_out << static_cast<int>(console_mode);
            }
            else if(static_cast<int>(console_mode) == 3)
            {
                conf_out << "keep";
            }
            else if(static_cast<int>(console_mode) == 4)
            {
                conf_out << "max";
            }
            else if(static_cast<int>(console_mode) == 5)
            {
                conf_out << "auto";
            }

            conf_out << endl
                << "editor " << editor << endl
                << "auto-entries " << auto_entries << endl
                << "beep " << beep << endl;

            conf_out.close();
        }
        else
        {
            throw runtime_error("Could not write to file. Config was not updated.");
        }
    }
    catch(const runtime_error& e)
    {
        cerr << e.what() << endl;
    }
    
}
void Global_Config::update()
{
    set_default("arch-linux");
    cout << "Updated default: arch-linux" << endl;
}
void Global_Config::print()
{
    if(file_loaded)
    {
        cout << "Current Options Loaded from loader.conf: " << endl
            << "Filepath: " << filepath << endl
            << "Path Definded: " << path_defined << " File Loaded: " << file_loaded << endl
            << "Default: " << default << endl
            << "Timeout: " << timeout << endl
            << "Console Mode: " << static_cast<int>(console_mode) << endl
            << "Editor: " << editor << endl
            << "Auto Entries: " << auto_entries << endl
            << "Beep: " << beep << endl;
    }
    else
    {
        
    }
}