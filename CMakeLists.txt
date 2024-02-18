cmake_minimum_required(VERSION 3.15)
project(NSSM)

set(CMAKE_CXX_STANDARD 20)
#Uncomment to force analyzer usa in GCC/CLANG.
#set(CXX_ANALYZER true)
SET(CMAKE_GENERATOR_PLATFORM x64)
string(TOLOWER "${CMAKE_CXX_COMPILER_ID}" COMPILER_ID)
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_ID)
message(STATUS "Compiler ID string is: ${COMPILER_ID}")
set(CMAKE_MC_COMPILER="C:/msys64/mingw64/bin/windmc.exe")


add_compile_definitions(DUNICODE -DUNICODE -D_UNICODE D_UNICODE)
if (COMPILER_ID STREQUAL "clang")
    message(STATUS "CLANG detected. Adding CLANG specific compile flags.")
    add_link_options("-fuse-ld=lld")
    set(IS_CLANG_BUILD true)
    #set(CMAKE_CXX_FLAGS "-fsanitize=undefined,bounds,integer ${CMAKE_CXX_FLAGS} -Wthread-safety")
    set(CMAKE_CXX_FLAGS "-fsanitize=address -fsanitize-address-use-after-return=always -fno-omit-frame-pointer ${CMAKE_CXX_FLAGS} -Wthread-safety")
    #set(CMAKE_CXX_FLAGS "-fuse-ld=mold")
else ()
    set(IS_CLANG_BUILD false)
endif ()

if (COMPILER_ID STREQUAL "gnu")
    message(STATUS "GCC detected. Adding GCC specific compile flags.")
    set(IS_GCC_BUILD true)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond -Wunsafe-loop-optimizations -Wuseless-cast -Wlogical-op -Waggressive-loop-optimizations -Wstrict-overflow=2 -Wbidi-chars=any -Wduplicated-branches -Wduplicated-cond -Wimplicit-fallthrough=5 -Wimplicit-fallthrough=5 -Wformat-truncation=2 -Wformat-signedness -Wlogical-op -Wformat-overflow=2 -Wtrivial-auto-var-init -Wshift-overflow=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
else ()
    set(IS_GCC_BUILD false)
endif ()

if (CXX_ANALYZER)
    if (IS_GCC_BUILD)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fanalyzer")
    endif ()
    if (IS_CLANG_BUILD)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -Xanalyzer -analyze")
    endif ()
endif ()

if (CMAKE_COMPILER_IS_GNUCXX OR IS_CLANG_BUILD OR IS_GCC_BUILD)
    message(STATUS "GCC/CLANG detected, adding compile flags")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Og")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -municode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdouble-promotion -Wnull-dereference -fdelete-null-pointer-checks -Wimplicit-fallthrough -Wuninitialized -Wsuggest-override -Wfloat-equal -Wcast-align -Wwrite-strings -Wconversion -Wzero-as-null-pointer-constant -Wparentheses -Wold-style-cast -Wshadow -Wformat=2 -Wformat-security -Wmissing-include-dirs -Wshift-overflow -Wunused-parameter -Wfloat-equal -Wcast-qual -Wcast-align -Wwrite-strings  -Wmissing-declarations")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-default") # Causes many warnings in third-party headers.
endif (CMAKE_COMPILER_IS_GNUCXX OR IS_CLANG_BUILD OR IS_GCC_BUILD)

if (BUILD_TYPE_ID STREQUAL "release")
    message(STATUS "Enabling LTO")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif (BUILD_TYPE_ID STREQUAL "release")

set(CMAKE_CXX_STANDARD 20)
set(SOURCES account.cpp console.cpp env.cpp event.cpp gui.cpp hook.cpp imports.cpp io.cpp nssm.cpp process.cpp registry.cpp service.cpp settings.cpp utf8.cpp
        helper.cpp

)

set(SOURCES2 src/helper.cpp src/console.cpp
        src/nssm.cpp
        src/imports.cpp
        src/service.cpp
        src/event.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} -lshlwapi)
target_compile_definitions(${PROJECT_NAME} PUBLIC DUNICODE)

add_executable(${PROJECT_NAME}2 ${SOURCES2})
target_link_libraries(${PROJECT_NAME}2 -lshlwapi)
target_include_directories(${PROJECT_NAME}2 PUBLIC inc)