cmake_minimum_required(VERSION 3.14)

project(imgup)

add_compile_options(-W -Wall)

find_package(nlohmann_json 3.6.1 REQUIRED)
find_package(CURL 7.64 REQUIRED)

add_executable(imgup
    src/imgup.hpp
    src/imgup.cpp
    src/request.hpp
    src/request.cpp
    src/main.cpp
    )

target_compile_features(imgup PUBLIC cxx_std_17)

target_link_libraries(imgup PRIVATE nlohmann_json::nlohmann_json
                            PRIVATE curl
    )
