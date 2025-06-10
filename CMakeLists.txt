cmake_minimum_required(VERSION 3.26)
project(RabbitAndSteel)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0135 NEW)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# 停用 PTSD 的自動更新，使用已經修改過的本地版本
# 只有當 PTSD 目錄不存在時才下載
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/PTSD")
    FetchContent_Declare(
        ptsd
        URL         https://github.com/ntut-open-source-club/practical-tools-for-simple-design/archive/refs/tags/v0.2.zip
        SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/PTSD
    )
    FetchContent_MakeAvailable(ptsd)
else()
    # 如果 PTSD 目錄已存在，直接使用本地版本
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/PTSD)
endif()

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
