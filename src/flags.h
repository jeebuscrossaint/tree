//
// Created by amarnath on 12/31/2025.
//

#ifndef TREE_FLAGS_H
#define TREE_FLAGS_H
#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <vector>

enum class FlagOptions {
    help,           // --help: Outputs a verbose usage listing
    version,        // --version: Outputs the version of tree
    all,            // -a: All files are printed, including hidden files
    directories_only, // -d: List directories only
    full_path,      // -f: Prints the full path prefix for each file
    no_indent,      // -i: Makes tree not print the indentation lines
    follow_symlinks, // -l: Follows symbolic links if they point to directories
    stay_filesystem, // -x: Stay on the current file-system only
    pattern,        // -P: List only those files that match the wild-card pattern
    ignore_pattern, // -I: Do not list those files that match the wild-card pattern
    no_report,      // --noreport: Omits printing of the file and directory report
    permissions,    // -p: Print the file type and permissions for each file
    size,           // -s: Print the size of each file in bytes
    human_readable, // -h: Print the size in human readable format
    username,       // -u: Print the username or UID
    group,          // -g: Print the group name or GID
    date,           // -D: Print the date of last modification
    inodes,         // --inodes: Prints the inode number
    device,         // --device: Prints the device number
    file_type,      // -F: Append indicators for file types
    quote_non_printable, // -q: Print non-printable characters as question marks
    print_non_printable, // -N: Print non-printable characters as is
    sort_version,   // -v: Sort the output by version
    reverse,        // -r: Sort the output in reverse alphabetic order
    sort_time,      // -t: Sort the output by last modification time
    dirs_first,     // --dirsfirst: List directories before files
    no_color,       // -n: Turn colorization off always
    color,          // -C: Turn colorization on always
    ansi_graphics,  // -A: Turn on ANSI line graphics
    ascii_graphics, // -S: Turn on ASCII line graphics
    level,          // -L: Max display depth of the directory tree
    file_limit      // --filelimit: Do not descend directories with more than # entries
};

struct FlagValues {
    bool help = false;
    bool version = false;
    bool all = false;
    bool directories_only = false;
    bool full_path = false;
    bool no_indent = false;
    bool follow_symlinks = false;
    bool stay_filesystem = false;
    std::optional<std::string> pattern;
    std::optional<std::string> ignore_pattern;
    bool no_report = false;
    bool permissions = false;
    bool size = false;
    bool human_readable = false;
    bool username = false;
    bool group = false;
    bool date = false;
    bool inodes = false;
    bool device = false;
    bool file_type = false;
    bool quote_non_printable = false;
    bool print_non_printable = false;
    bool sort_version = false;
    bool reverse = false;
    bool sort_time = false;
    bool dirs_first = false;
    bool no_color = false;
    bool color = false;
    bool ansi_graphics = false;
    bool ascii_graphics = false;
    std::optional<int> level;
    std::optional<int> file_limit;
    std::vector<std::string> paths;
};

FlagValues parse_flags(int argc, char* argv[]);
void print_help();
void print_version();

inline std::string_view to_string(FlagOptions flag)
{
    switch (flag)
    {
    case FlagOptions::help: return "--help";
    case FlagOptions::version: return "--version";
    case FlagOptions::all: return "-a";
    case FlagOptions::directories_only: return "-d";
    case FlagOptions::full_path: return "-f";
    case FlagOptions::no_indent: return "-i";
    case FlagOptions::follow_symlinks: return "-l";
    case FlagOptions::stay_filesystem: return "-x";
    case FlagOptions::pattern: return "-P";
    case FlagOptions::ignore_pattern: return "-I";
    case FlagOptions::no_report: return "--noreport";
    case FlagOptions::permissions: return "-p";
    case FlagOptions::size: return "-s";
    case FlagOptions::human_readable: return "-h";
    case FlagOptions::username: return "-u";
    case FlagOptions::group: return "-g";
    case FlagOptions::date: return "-D";
    case FlagOptions::inodes: return "--inodes";
    case FlagOptions::device: return "--device";
    case FlagOptions::file_type: return "-F";
    case FlagOptions::quote_non_printable: return "-q";
    case FlagOptions::print_non_printable: return "-N";
    case FlagOptions::sort_version: return "-v";
    case FlagOptions::reverse: return "-r";
    case FlagOptions::sort_time: return "-t";
    case FlagOptions::dirs_first: return "--dirsfirst";
    case FlagOptions::no_color: return "-n";
    case FlagOptions::color: return "-C";
    case FlagOptions::ansi_graphics: return "-A";
    case FlagOptions::ascii_graphics: return "-S";
    case FlagOptions::level: return "-L";
    case FlagOptions::file_limit: return "--filelimit";
    default: return "";
    }
}


#endif //TREE_FLAGS_H
