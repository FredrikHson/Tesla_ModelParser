cmake_minimum_required(VERSION 2.8)

project(teslamodelparser)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(COMMON_SRC
    "src/main.cpp"
    )

include_directories(
    "src/"
    )

add_executable(${CMAKE_PROJECT_NAME} ${COMMON_SRC})

target_link_libraries(${CMAKE_PROJECT_NAME}
    )
