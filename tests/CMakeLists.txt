# This file must be included in every CMakeLists.txt in the subfolders

message(STATUS "Building Unit Tests")

# GoogleTest 1.12.x requires at least C++11
# Future releases will need C++14 or higher
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (NOT MSVC)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
IF (WIN32)
add_compile_options(-mno-ms-bitfields)
ENDIF()
ENDIF()

# Fetch GoogleTest 1.12.1
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(../../src)

enable_testing()

include(GoogleTest)
