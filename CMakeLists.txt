cmake_minimum_required(VERSION 3.10)

project("water_simulation")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check for fmt library
find_package(fmt QUIET)

if(NOT fmt_FOUND)
    message(WARNING "fmt library not found. Falling back to iostream.")
    # Define a preprocessor macro to use iostream
    add_definitions(-DUSE_IOSTREAM)
else()
    # If fmt is found, link it
    message(STATUS "fmt library found.")
endif()

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HDR_FILES "src/*.h")

# Define the executable target
add_executable("water_simulation" main.cpp ${SRC_FILES} ${HDR_FILES})

# Include directories
target_include_directories("water_simulation" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Compile options
target_compile_options("water_simulation" PRIVATE 
    -Wall
    -Wextra
    -Wshadow
    -Wswitch
    -pedantic
    -Wformat=2
    -Wconversion
    -Wnull-dereference
    -Wunused-parameter
    -Wunreachable-code
    -Wimplicit-fallthrough
    -Werror
    -Werror=return-type
    -Werror=uninitialized
    -Werror=unused-result
    -Werror=strict-overflow
    -fsanitize=address
    -fsanitize=undefined
    -fno-omit-frame-pointer
)

# Link fmt library if found
if(fmt_FOUND)
    target_link_libraries("water_simulation" PRIVATE fmt::fmt)
endif()

# Link options for sanitizers
target_link_options("water_simulation" PRIVATE
    -fsanitize=address
    -fsanitize=undefined
)

target_compile_features("water_simulation" PRIVATE cxx_std_20)

# If not Visual Studio generator, copy compile_commands.json
if(NOT CMAKE_GENERATOR MATCHES "Visual Studio")
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
        DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
        COMMENT "Copying compile_commands.json..."
    )

    add_custom_target(copy_compile_commands ALL
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif()

# Add a CMake option to control running the Python script
option(RUN_GRAPH_SCRIPT "Run the show_graph.py script after building" OFF)

if(RUN_GRAPH_SCRIPT)
    # Ensure the virtual environment is activated and dependencies are installed before running the script
    add_custom_command(
        TARGET water_simulation
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Activating virtual environment..."
        COMMAND bash -c "source ${CMAKE_CURRENT_SOURCE_DIR}/venv/bin/activate && pip install -r ${CMAKE_CURRENT_SOURCE_DIR}/requirements.txt"
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/show_graph.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running Python script with virtual environment..."
    )
endif()
