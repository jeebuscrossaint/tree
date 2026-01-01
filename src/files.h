//
// Created by amarnath on 1/1/2026.
//

#ifndef TREE_FILES_H
#define TREE_FILES_H

/*
 * As per the tree command man page
 * Files
 * /etc/DIR_COLORS
 * System color database.
 * ~/.dircolors
 * Users color database.
*/

/*
 * Since this is obviously windows write there is no /etc/dircolors duh
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <cstdlib>

struct color_mapping
{
 std::string type; // FILE, DIR, LINK, etc. or file extension
 std::string color_code; // e.g. "01;34"
};

std::unordered_map<std::string, std::string> parse_dircolors();


#endif //TREE_FILES_H