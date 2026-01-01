//
// Created by amarnath on 1/1/2026.
//

#ifndef TREE_ENV_H
#define TREE_ENV_H

/* As per the tree man page
 * Environment
 * LS_COLORS
 * Color information created by dircolors
 * TREE_CHARSET
 * Character set for tree to use in HTML mode.
 * LC_CTYPE
 * Locale for filename output.
*/

#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

// Parse LS_COLORS environment variable
std::unordered_map<std::string, std::string> parse_ls_colors();

#endif //TREE_ENV_H