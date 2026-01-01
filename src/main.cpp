//
// Created by amarnath on 12/31/2025.
//

#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <regex>
#include <windows.h>
#include "flags.h"
#include "files.h"
#include "env.h"

namespace fs = std::filesystem;

struct TreeStats {
    size_t dir_count = 0;
    size_t file_count = 0;
};

class TreePrinter {
private:
    FlagValues flags;
    TreeStats stats;
    std::unordered_map<std::string, std::string> colors;
    bool use_color = false;

    std::string get_color_code(const std::string& key) {
        if (!use_color) return "";
        auto it = colors.find(key);
        if (it != colors.end()) {
            return "\033[" + it->second + "m";
        }
        return "";
    }

    std::string reset_color() {
        return use_color ? "\033[0m" : "";
    }

    std::string get_file_color(const fs::path& path) {
        if (!use_color) return "";

        if (fs::is_directory(path)) {
            return get_color_code("DIR");
        } else if (fs::is_symlink(path)) {
            return get_color_code("LINK");
        } else if (fs::is_regular_file(path)) {
            // Check extension
            std::string ext = path.extension().string();
            if (!ext.empty()) {
                auto it = colors.find(ext);
                if (it != colors.end()) {
                    return "\033[" + it->second + "m";
                }
            }
            // Check if executable
            #ifdef _WIN32
            if (ext == ".exe" || ext == ".bat" || ext == ".cmd") {
                return get_color_code("EXEC");
            }
            #else
            struct stat st;
            if (stat(path.string().c_str(), &st) == 0 && (st.st_mode & S_IXUSR)) {
                return get_color_code("EXEC");
            }
            #endif
        }
        return "";
    }

    std::string format_size(uintmax_t size) {
        if (flags.human_readable) {
            const char* units[] = {"", "K", "M", "G", "T", "P", "E"};
            int unit = 0;
            double dsize = static_cast<double>(size);

            while (dsize >= 1024.0 && unit < 6) {
                dsize /= 1024.0;
                unit++;
            }

            std::ostringstream oss;
            if (unit == 0) {
                oss << size;
            } else {
                oss << std::fixed << std::setprecision(1) << dsize << units[unit];
            }
            return oss.str();
        } else {
            return std::to_string(size);
        }
    }

    std::string format_permissions(const fs::path& path) {
        std::string result;

        if (fs::is_directory(path)) result += "d";
        else if (fs::is_symlink(path)) result += "l";
        else result += "-";

        #ifdef _WIN32
        // Windows simplified permissions
        DWORD attrs = GetFileAttributesA(path.string().c_str());
        if (attrs != INVALID_FILE_ATTRIBUTES) {
            result += (attrs & FILE_ATTRIBUTE_READONLY) ? "r-x" : "rwx";
            result += (attrs & FILE_ATTRIBUTE_READONLY) ? "r-x" : "rwx";
            result += (attrs & FILE_ATTRIBUTE_READONLY) ? "r-x" : "rwx";
        } else {
            result += "rwxrwxrwx";
        }
        #else
        struct stat st;
        if (stat(path.string().c_str(), &st) == 0) {
            result += (st.st_mode & S_IRUSR) ? "r" : "-";
            result += (st.st_mode & S_IWUSR) ? "w" : "-";
            result += (st.st_mode & S_IXUSR) ? "x" : "-";
            result += (st.st_mode & S_IRGRP) ? "r" : "-";
            result += (st.st_mode & S_IWGRP) ? "w" : "-";
            result += (st.st_mode & S_IXGRP) ? "x" : "-";
            result += (st.st_mode & S_IROTH) ? "r" : "-";
            result += (st.st_mode & S_IWOTH) ? "w" : "-";
            result += (st.st_mode & S_IXOTH) ? "x" : "-";
        }
        #endif

        return result;
    }

    std::string format_date(const fs::path& path) {
        try {
            auto ftime = fs::last_write_time(path);
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
            std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

            std::ostringstream oss;
            oss << std::put_time(std::localtime(&cftime), "%b %d %H:%M");
            return oss.str();
        } catch (...) {
            return "???";
        }
    }

    std::string get_file_type_indicator(const fs::path& path) {
        if (!flags.file_type) return "";

        if (fs::is_directory(path)) return "/";
        else if (fs::is_symlink(path)) return "@";
        #ifdef _WIN32
        std::string ext = path.extension().string();
        if (ext == ".exe" || ext == ".bat" || ext == ".cmd") return "*";
        #else
        struct stat st;
        if (stat(path.string().c_str(), &st) == 0) {
            if (S_ISFIFO(st.st_mode)) return "|";
            if (S_ISSOCK(st.st_mode)) return "=";
            if (st.st_mode & S_IXUSR) return "*";
        }
        #endif

        return "";
    }

    bool matches_pattern(const std::string& name, const std::string& pattern) {
        // Simple wildcard matching
        std::string regex_pattern = pattern;
        // Escape special regex chars except * and ?
        size_t pos = 0;
        while ((pos = regex_pattern.find(".", pos)) != std::string::npos) {
            regex_pattern.replace(pos, 1, "\\.");
            pos += 2;
        }
        // Convert wildcard to regex
        pos = 0;
        while ((pos = regex_pattern.find("*", pos)) != std::string::npos) {
            regex_pattern.replace(pos, 1, ".*");
            pos += 2;
        }
        pos = 0;
        while ((pos = regex_pattern.find("?", pos)) != std::string::npos) {
            regex_pattern.replace(pos, 1, ".");
            pos += 1;
        }

        try {
            std::regex re(regex_pattern);
            return std::regex_match(name, re);
        } catch (...) {
            return false;
        }
    }

    bool should_include(const fs::path& path) {
        std::string name = path.filename().string();

        // Skip hidden files unless -a flag is set
        if (!flags.all && !name.empty() && name[0] == '.') {
            return false;
        }

        // Check pattern matching
        if (flags.pattern && !matches_pattern(name, *flags.pattern)) {
            return false;
        }

        // Check ignore pattern
        if (flags.ignore_pattern && matches_pattern(name, *flags.ignore_pattern)) {
            return false;
        }

        return true;
    }

    void print_entry_info(const fs::path& path) {
        if (flags.permissions) {
            std::cout << "[" << format_permissions(path) << "] ";
        }

        if (flags.size || flags.human_readable) {
            try {
                if (fs::is_regular_file(path)) {
                    std::cout << std::setw(10) << format_size(fs::file_size(path)) << " ";
                } else {
                    std::cout << std::setw(10) << "" << " ";
                }
            } catch (...) {
                std::cout << std::setw(10) << "?" << " ";
            }
        }

        if (flags.date) {
            std::cout << format_date(path) << " ";
        }
    }

    void print_tree(const fs::path& path, const std::string& prefix, bool is_last, int depth) {
        if (flags.level && depth > *flags.level) {
            return;
        }

        try {
            // Don't filter root directory (depth == 0)
            if (depth > 0 && !should_include(path)) {
                return;
            }

            // Print the current entry (but not root)
            if (depth > 0) {
                if (!flags.no_indent) {
                    std::cout << prefix;

                    if (flags.ansi_graphics) {
                        std::cout << (is_last ? "└── " : "├── ");
                    } else if (flags.ascii_graphics) {
                        std::cout << (is_last ? "`-- " : "|-- ");
                    } else {
                        // Use Unicode box-drawing characters
                        std::cout << (is_last ? "└── " : "├── ");
                    }
                }

                print_entry_info(path);

                std::string color = get_file_color(path);
                std::string name = flags.full_path ? path.string() : path.filename().string();
                std::string indicator = get_file_type_indicator(path);

                std::cout << color << name << indicator << reset_color();

                if (fs::is_symlink(path)) {
                    try {
                        std::cout << " -> " << fs::read_symlink(path).string();
                    } catch (...) {}
                }

                std::cout << std::endl;
            }

            // Process directory contents
            if (fs::is_directory(path)) {
                // Collect entries
                std::vector<fs::path> entries;
                try {
                    for (const auto& entry : fs::directory_iterator(path)) {
                        if (should_include(entry.path())) {
                            entries.push_back(entry.path());
                        }
                    }
                } catch (const std::exception& e) {
                    return;
                }

                // Check file limit
                if (flags.file_limit && entries.size() > static_cast<size_t>(*flags.file_limit)) {
                    return;
                }

                // Sort entries
                std::sort(entries.begin(), entries.end(), [this](const fs::path& a, const fs::path& b) {
                    bool a_is_dir = fs::is_directory(a);
                    bool b_is_dir = fs::is_directory(b);

                    if (flags.dirs_first && a_is_dir != b_is_dir) {
                        return a_is_dir;
                    }

                    if (flags.sort_time) {
                        try {
                            return fs::last_write_time(a) > fs::last_write_time(b);
                        } catch (...) {
                            return a.filename().string() < b.filename().string();
                        }
                    }

                    std::string name_a = a.filename().string();
                    std::string name_b = b.filename().string();

                    if (flags.reverse) {
                        return name_a > name_b;
                    }

                    return name_a < name_b;
                });

                // Count and filter entries
                for (const auto& entry : entries) {
                    if (fs::is_directory(entry)) {
                        stats.dir_count++;
                    } else {
                        stats.file_count++;
                    }
                }

                // Filter directories if needed
                if (flags.directories_only) {
                    entries.erase(std::remove_if(entries.begin(), entries.end(),
                        [](const fs::path& p) { return !fs::is_directory(p); }), entries.end());
                }

                // Print entries
                for (size_t i = 0; i < entries.size(); ++i) {
                    bool is_last_entry = (i == entries.size() - 1);
                    std::string new_prefix = prefix;

                    if (!flags.no_indent) {
                        if (flags.ansi_graphics) {
                            new_prefix += is_last ? "    " : "│   ";
                        } else if (flags.ascii_graphics) {
                            new_prefix += is_last ? "    " : "|   ";
                        } else {
                            // Use Unicode box-drawing characters
                            new_prefix += is_last ? "    " : "│   ";
                        }
                    }

                    print_tree(entries[i], new_prefix, is_last_entry, depth + 1);
                }
            }

        } catch (const std::exception& e) {
            // Silently skip inaccessible entries
        }
    }

public:
    TreePrinter(const FlagValues& f) : flags(f) {
        // Determine if we should use color
        if (flags.color) {
            use_color = true;
        } else if (!flags.no_color) {
            // Check if stdout is a terminal
            #ifdef _WIN32
            DWORD mode;
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            use_color = GetConsoleMode(hOut, &mode);

            // Enable virtual terminal processing on Windows 10+
            if (use_color) {
                SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
            #else
            use_color = isatty(fileno(stdout));
            #endif
        }

        // Load colors
        if (use_color) {
            colors = parse_ls_colors();
            if (colors.empty()) {
                colors = parse_dircolors();
            }

            // Default colors if none loaded
            if (colors.empty()) {
                colors["DIR"] = "01;34";
                colors["LINK"] = "01;36";
                colors["EXEC"] = "01;32";
                colors["FIFO"] = "40;33";
                colors["SOCK"] = "01;35";
                colors[".tar"] = "01;31";
                colors[".zip"] = "01;31";
                colors[".gz"] = "01;31";
            }
        }
    }

    void print(const std::string& root_path) {
        fs::path path(root_path);

        if (!fs::exists(path)) {
            std::cerr << "Error: " << root_path << " does not exist" << std::endl;
            return;
        }

        // Print root
        std::string color = get_file_color(path);
        std::cout << color << path.string() << reset_color() << std::endl;

        print_tree(path, "", true, 0);

        // Print report
        if (!flags.no_report) {
            std::cout << std::endl;
            std::cout << stats.dir_count << " directories";
            if (!flags.directories_only) {
                std::cout << ", " << stats.file_count << " files";
            }
            std::cout << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    #ifdef _WIN32
    // Set console to UTF-8 mode on Windows
    SetConsoleOutputCP(CP_UTF8);
    // Enable buffering to prevent character issues
    setvbuf(stdout, nullptr, _IOFBF, 1000);
    #endif

    FlagValues flags = parse_flags(argc, argv);

    if (flags.help) {
        print_help();
        return 0;
    }

    if (flags.version) {
        print_version();
        return 0;
    }

    TreePrinter printer(flags);

    for (const auto& path : flags.paths) {
        printer.print(path);
    }

    return 0;
}