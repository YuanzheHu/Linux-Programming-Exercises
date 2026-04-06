# minicloc

`minicloc` is a lightweight C++ command-line tool for counting lines of code in a file or directory.

It recursively scans files, classifies each line as `code`, `comment`, or `blank`, then prints:
- Per-language summary table
- Optional per-file detail table (`--show-files`)

## Features

- Analyze a single file or an entire directory tree
- Extension-based filtering with `--ext`
- Language detection by file extension
- Recursive traversal with skip rules for common build/dependency folders
- Terminal-friendly aligned report tables

## Project Structure

- `main.cpp`: program entry and argument parsing flow
- `cli.h`, `cli.cpp`: help text and extension list parsing
- `stats.h`: `FileStats` and `LangStats`
- `language.h`, `language.cpp`: extension extraction and language detection
- `line_classifier.h`, `line_classifier.cpp`: trim + line classification logic
- `file_analyzer.h`, `file_analyzer.cpp`: per-file read/analyze implementation
- `file_collector.h`, `file_collector.cpp`: recursive file collection
- `report.h`, `report.cpp`: formatted report output
- `CMakeLists.txt`: CMake build config

## Build (CMake)

```bash
cd /home/yuanzhe/linux-programming-exercises/minicloc
cmake -S . -B build
cmake --build build
```

Binary path:
- `./build/minicloc`

## Usage

```bash
./build/minicloc <path> [options]
```

### Options

- `--ext <exts>`: only analyze specified extensions, comma-separated (e.g. `cpp,h,c`)
- `--show-files`: show per-file detail table
- `--help` or `-h`: show help

## Examples

```bash
./build/minicloc main.cpp
./build/minicloc .
./build/minicloc . --ext cpp,h
./build/minicloc . --ext cpp --show-files
```

## Command Examples with Output

### 1) Show help

```bash
./build/minicloc --help
```

Example output:

```text
Usage: ./build/minicloc <path> [options]

Arguments:
	<path>            File or directory to analyze

Options:
	--ext <ext>       Only analyze the specified extensions, separated by commas (e.g. cpp,h,c)
	--show-files      Show detailed statistics for each file
	--help            Show this help message
```

### 2) Analyze C++ files recursively and show per-file details

```bash
./build/minicloc . --ext cpp --show-files
```

Example output:

```text
┌─────────────────────────────────────────────────────────────────┐
│                  File Details                                   │
├──────────────────────────────────────┬───────┬────────┬─────────┤
│ File                                 │ Total │ Code   │ Comment │
├──────────────────────────────────────┼───────┼────────┼─────────┤
│ ./cli.cpp                            │    39 │     31 │       5 │
│ ./file_collector.cpp                 │    70 │     52 │       7 │
│ ./main.cpp                           │    79 │     60 │       9 │
│ ...                                  │   ... │    ... │     ... │
└──────────────────────────────────────┴───────┴────────┴─────────┘

┌──────────────────────────────┬───────┬───────────┬─────────┬─────────┬───────┐
│ Language                     │ Files │   Total   │  Code   │ Comment │ Blank │
├──────────────────────────────┼───────┼───────────┼─────────┼─────────┼───────┤
│ C++                          │     7 │       441 │     348 │      38 │    55 │
├──────────────────────────────┼───────┼───────────┼─────────┼─────────┼───────┤
│ Total                        │     7 │       441 │     348 │      38 │    55 │
└──────────────────────────────┴───────┴───────────┴─────────┴─────────┴───────┘
```

### 3) Analyze only header files

```bash
./build/minicloc . --ext h
```

Example output:

```text
┌──────────────────────────────┬───────┬───────────┬─────────┬─────────┬───────┐
│ Language                     │ Files │   Total   │  Code   │ Comment │ Blank │
├──────────────────────────────┼───────┼───────────┼─────────┼─────────┼───────┤
│ C/C++ Header                 │     7 │        79 │      41 │      19 │    19 │
├──────────────────────────────┼───────┼───────────┼─────────┼─────────┼───────┤
│ Total                        │     7 │        79 │      41 │      19 │    19 │
└──────────────────────────────┴───────┴───────────┴─────────┴─────────┴───────┘
```

## How it works

1. Read target path (`file` or `directory`)
2. Collect candidate files recursively (`stat`, `opendir`, `readdir`)
3. Filter by extension if `--ext` is set
4. Analyze each file line-by-line (`open`, `read`, `close`)
5. Classify each line as `CODE`, `COMMENT`, or `BLANK`
6. Aggregate stats by language and print tables
