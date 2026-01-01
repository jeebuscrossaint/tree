//
// Created by amarnath on 12/31/2025.
//

#include "flags.h"
#include <iostream>
#include <cstring>

void print_version() {
    std::cout << "tree v∞.0 - \"It's not a bug, it's a feature-complete tree!\"" << std::endl;
    std::cout << "Copyright (C) 2025-2026 - Now with 100% more branches than your git repo" << std::endl;
}

void print_help() {
    std::cout << R"(usage: tree [options] [directory...]

Options:
  --help              Outputs a verbose usage listing.
  --version           Outputs the version of tree.
  -a                  All files are printed (including hidden files).
  -d                  List directories only.
  -f                  Prints the full path prefix for each file.
  -i                  Makes tree not print the indentation lines.
  -l                  Follows symbolic links if they point to directories.
  -x                  Stay on the current file-system only.
  -P pattern          List only those files that match the wild-card pattern.
  -I pattern          Do not list those files that match the wild-card pattern.
  --noreport          Omits printing of the file and directory report.
  -p                  Print the file type and permissions for each file.
  -s                  Print the size of each file in bytes.
  -h                  Print the size in human readable format.
  -u                  Print the username or UID.
  -g                  Print the group name or GID.
  -D                  Print the date of last modification.
  --inodes            Prints the inode number.
  --device            Prints the device number.
  -F                  Append indicators for file types (/, =, *, |).
  -q                  Print non-printable characters as question marks.
  -N                  Print non-printable characters as is.
  -v                  Sort the output by version.
  -r                  Sort the output in reverse alphabetic order.
  -t                  Sort the output by last modification time.
  --dirsfirst         List directories before files.
  -n                  Turn colorization off always.
  -C                  Turn colorization on always.
  -A                  Turn on ANSI line graphics.
  -S                  Turn on ASCII line graphics.
  -L level            Max display depth of the directory tree.
  --filelimit #       Do not descend directories with more than # entries.
)" << std::endl;
}

FlagValues parse_flags(int argc, char* argv[]) {
    FlagValues flags;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help") {
            flags.help = true;
        } else if (arg == "--version") {
            flags.version = true;
        } else if (arg == "-a") {
            flags.all = true;
        } else if (arg == "-d") {
            flags.directories_only = true;
        } else if (arg == "-f") {
            flags.full_path = true;
        } else if (arg == "-i") {
            flags.no_indent = true;
        } else if (arg == "-l") {
            flags.follow_symlinks = true;
        } else if (arg == "-x") {
            flags.stay_filesystem = true;
        } else if (arg == "-P" && i + 1 < argc) {
            flags.pattern = argv[++i];
        } else if (arg == "-I" && i + 1 < argc) {
            flags.ignore_pattern = argv[++i];
        } else if (arg == "--noreport") {
            flags.no_report = true;
        } else if (arg == "-p") {
            flags.permissions = true;
        } else if (arg == "-s") {
            flags.size = true;
        } else if (arg == "-h") {
            flags.human_readable = true;
        } else if (arg == "-u") {
            flags.username = true;
        } else if (arg == "-g") {
            flags.group = true;
        } else if (arg == "-D") {
            flags.date = true;
        } else if (arg == "--inodes") {
            flags.inodes = true;
        } else if (arg == "--device") {
            flags.device = true;
        } else if (arg == "-F") {
            flags.file_type = true;
        } else if (arg == "-q") {
            flags.quote_non_printable = true;
        } else if (arg == "-N") {
            flags.print_non_printable = true;
        } else if (arg == "-v") {
            flags.sort_version = true;
        } else if (arg == "-r") {
            flags.reverse = true;
        } else if (arg == "-t") {
            flags.sort_time = true;
        } else if (arg == "--dirsfirst") {
            flags.dirs_first = true;
        } else if (arg == "-n") {
            flags.no_color = true;
        } else if (arg == "-C") {
            flags.color = true;
        } else if (arg == "-A") {
            flags.ansi_graphics = true;
        } else if (arg == "-S") {
            flags.ascii_graphics = true;
        } else if (arg == "-L" && i + 1 < argc) {
            flags.level = std::stoi(argv[++i]);
        } else if (arg == "--filelimit" && i + 1 < argc) {
            flags.file_limit = std::stoi(argv[++i]);
        } else if (arg[0] != '-') {
            flags.paths.push_back(arg);
        }
    }

    // Default to current directory if no paths specified
    if (flags.paths.empty()) {
        flags.paths.push_back(".");
    }

    return flags;
}
