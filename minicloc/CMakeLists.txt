cmake_minimum_required(VERSION 3.16)
project(minicloc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(minicloc
    main.cpp
    cli.cpp
    language.cpp
    line_classifier.cpp
    file_analyzer.cpp
    file_collector.cpp
    report.cpp
)
