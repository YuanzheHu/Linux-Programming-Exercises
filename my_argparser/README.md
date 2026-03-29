# fileinfo Tool (Brief Guide)

`fileinfo` is a small command-line tool that shows Linux file metadata using the `stat()` system call.

## Project Structure

- `fileinfo.cpp` : Program entry (`main`)
- `arg_parser.h`, `arg_parser.cpp` : Command-line argument parser
- `file_info.h`, `file_info.cpp` : File metadata logic based on `stat()`
- `CMakeLists.txt` : CMake build configuration

## Build (Recommended: CMake)

```bash
cmake -S . -B build
cmake --build build
```

The executable will be generated at `build/fileinfo`.

## Build (Single g++ Command)

```bash
g++ -std=c++17 fileinfo.cpp arg_parser.cpp file_info.cpp -o fileinfo
```

## Basic Usage

```bash
./build/fileinfo --file <path>
```

Example:

```bash
./build/fileinfo --file fileinfo.cpp
```

## Options

- `-h`, `--help` : Show help message
- `-V`, `--version` : Show version and process ID
- `-f`, `--file <path>` : Target file path (required for file info)
- `-v`, `--verbose` : Show more metadata fields
- `--format <normal|json>` : Output format (default: `normal`)

## Common Examples

Normal output:

```bash
./build/fileinfo -f fileinfo.cpp
```

Verbose output:

```bash
./build/fileinfo -f fileinfo.cpp -v
```

JSON output:

```bash
./build/fileinfo -f fileinfo.cpp --format json
```

## How It Works

The tool calls `stat()` to read file metadata, then prints:

- File type
- File size
- Permission bits
- Last modified time

With `-v`, it also prints inode, hard links, owner UID, and owner GID.

## Troubleshooting

If compilation fails, recompile with warnings to locate syntax issues quickly:

```bash
g++ -Wall -Wextra -std=c++17 fileinfo.cpp arg_parser.cpp file_info.cpp -o fileinfo
```

If you see `Cannot access file`, check whether the path exists and is readable.
