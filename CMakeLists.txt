cmake_minimum_required(VERSION 3.26)
project(RabbitAndSteel)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0135 NEW)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    ptsd

    URL         https://github.com/ntut-open-source-club/practical-tools-for-simple-design/archive/refs/tags/v0.2.zip

    # Or alternatively if you want to use a specific git commit instead of a release:
    # GIT_REPOSITORY  https://github.com/ntut-open-source-club/practical-tools-for-simple-design.git
    # GIT_TAG         <git_tag>

    SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/PTSD
)

FetchContent_MakeAvailable(ptsd)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.hpp)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES}
        include/Attack/Attack.hpp
        include/Attack/AttackPattern.hpp
        src/Attack/Attack.cpp
        src/Attack/AttackPattern.cpp
        include/Attack/CircleAttack.hpp
        src/Attack/CircleAttack.cpp
        include/Attack/RectangleAttack.hpp
        src/Attack/RectangleAttack.cpp
        include/Attack/AttackPatternFactory.hpp
        src/Attack/AttackPatternFactory.cpp
        include/Attack/EnemyAttackController.hpp
        src/Attack/EnemyAttackController.cpp
        include/Attack/CornerBulletAttack.hpp
        src/Attack/CornerBulletAttack.cpp
        include/Attack/AttackManager.hpp
        src/Attack/AttackManager.cpp
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE GA_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE GA_RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/Resources")
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${DEPENDENCY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2main
    PTSD
)
