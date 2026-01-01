//
// Created by amarnath on 1/1/2026.
//

#include "files.h"

std::unordered_map<std::string, std::string> parse_dircolors()
{
    std::unordered_map<std::string, std::string> color_map;

    // get home directory path
    const char* home = std::getenv("USERPROFILE"); // windows
    if (!home) home = std::getenv("HOME"); // fallback

    if (!home) return color_map;

    std::string dircolors_path = std::string(home) + "/.dircolors";
    std::ifstream file(dircolors_path);

    if (!file.is_open())
    {
        // Silently return empty map if file doesn't exist
        return color_map;
    }

    std::string line;
    while (std::getline(file, line))
    {
        // skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        // skip TERM and COLORTERM entries (they are terminal specific)
        if (line.find("TERM ") == 0 || line.find("COLORTERM") == 0) continue;

        std::istringstream iss(line);
        std::string key, value;

        if (iss >> key >> value)
        {
            // store mapping
            color_map[key] = value;
        }
    }

    file.close();
    return color_map;
}