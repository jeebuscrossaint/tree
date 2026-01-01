//
// Created by amarnath on 1/1/2026.
//

#include "env.h"

std::unordered_map<std::string, std::string> parse_ls_colors()
{
    std::unordered_map<std::string, std::string> color_map;

    const char* ls_colors = std::getenv("LS_COLORS");
    if (!ls_colors) return color_map;

    std::string colors_str(ls_colors);
    std::istringstream stream(colors_str);
    std::string token;

    while (std::getline(stream, token, ':'))
    {
        if (token.empty()) continue;

        size_t eq_pos = token.find('=');
        if (eq_pos != std::string::npos)
        {
            std::string key = token.substr(0, eq_pos);
            std::string value = token.substr(eq_pos + 1);
            color_map[key] = value;
        }
    }

    return color_map;
}

