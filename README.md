# Tree Command - Windows Implementation

A 1:1 implementation of the Linux `tree` command for Windows, written in C++20.

## Features

This is a complete port of the Linux tree command with **all** standard features except the HTML-related flags (`-H`, `-T`, `--charset`, `--nolinks`, `-R`). All other functionality works identically to the Linux version.

### What's Included

- All listing options (`-a`, `-d`, `-l`, `-f`, `-i`, `-F`, `--noreport`, etc.)
- All file information flags (`-p`, `-s`, `-h`, `-u`, `-g`, `-D`, `--inodes`, `--device`)
- Pattern matching and filtering (`-P`, `-I`, `--filelimit`)
- All sorting options (`-r`, `-t`, `-v`, `--dirsfirst`)
- Full color support (`-C`, `-n`) with LS_COLORS and .dircolors parsing
- Graphics options (`-A` for Unicode, `-S` for ASCII)
- Advanced features (`-x`, `-L`, `-l`)

### Box-Drawing Characters

By default, uses Unicode box-drawing characters (`├──`, `└──`, `│`). The console is automatically set to UTF-8 mode on Windows.

Use `-S` for ASCII fallback (`|--`, `` `-- ``, `|`) when needed for compatibility.

## Installation

### Quick Install (Scoop - Recommended)

The easiest way to install is using [Scoop](https://scoop.sh):

```powershell
# Create a local bucket for personal apps
mkdir ~\scoop-bucket
cd ~\scoop-bucket
git init

# Copy the manifest from this repo
cp path\to\tree.json .\tree.json
git add tree.json
git commit -m "Add tree"

# Add bucket and install
scoop bucket add myapps ~\scoop-bucket
scoop install myapps/tree
```

See [INSTALL.md](INSTALL.md) for more installation options including manual installation and GitHub releases setup.

## Building

### Requirements
- CMake 4.1+
- MinGW with GCC (C++20 support)
- Windows 10+

### Build Instructions

```powershell
# Create build directory
mkdir cmake-build-debug
cd cmake-build-debug

# Configure with MinGW Makefiles
cmake -G "MinGW Makefiles" ..

# Build
cmake --build .
```

The executable will be created as `tree.exe` in the build directory.

### Build Features
- Static linking of libgcc and libstdc++ (no runtime DLL dependencies)
- UTF-8 console output support
- Optimized file system traversal

## Usage

Works exactly like the Linux `tree` command. Run `tree --help` for the full list of options.

## Differences from Linux tree

- Permissions are simplified on Windows (no true Unix permission bits)
- Username/group display is placeholder (Windows uses different security model)
- Inode/device numbers are placeholders (different filesystem concepts)
- Symbolic links are less common on Windows but supported
- HTML-related flags (`-H`, `-T`, `--charset`, `--nolinks`, `-R`) are not implemented

## Known Issues

### PowerShell Pipeline Encoding

When piping the output through PowerShell cmdlets like `Select-String`, `Select-Object`, or `Where-Object`, the UTF-8 box-drawing characters may appear mangled:

```powershell
# This will show mangled characters like "ΓööΓöÇΓöÇ"
.\tree.exe | Select-String "something"

# Direct output works fine
.\tree.exe
```

**Workarounds:**

1. **Use ASCII graphics mode** when you need to pipe output:
   ```powershell
   .\tree.exe -S | Select-String "something"
   ```

2. **Redirect to file** instead of piping:
   ```powershell
   .\tree.exe > output.txt
   type output.txt | Select-String "something"
   ```

3. **Set PowerShell encoding** before piping (may not always work):
   ```powershell
   [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
   .\tree.exe | Select-String "something"
   ```

4. **Use without piping** and filter manually or use `findstr` instead:
   ```cmd
   tree.exe | findstr "something"
   ```

This is a limitation of PowerShell's pipeline encoding handling, not the tree program itself. The output is correct UTF-8, but PowerShell's internal pipeline doesn't always preserve it properly.



