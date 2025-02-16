# version 3.11 or later of CMake or needed later for installing GoogleTest
# so let's require it now.
cmake_minimum_required(VERSION 3.12-3.28)

project(callbask-test)

# Debug options
add_compile_options(-g -O0)

# Setup Python
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(Boost REQUIRED COMPONENTS python3)


add_library(callback MODULE callback_module.cpp)
set_target_properties(callback PROPERTIES
    PREFIX "${PYTHON_MODULE_PREFIX}"
)
 
# Setup file extensiton for Windows
if (WIN32)
    add_target_properties(callback PROPERTIES
        SUFFIX ".pyd"
    )
endif()

target_include_directories(callback
    PRIVATE ${Python3_INCLUDE_DIRS}
)

target_link_libraries(callback
    ${Python3_LIBRARIES}
    ${Boost_LIBRARIES}
)

target_compile_features(callback PUBLIC cxx_std_20)

# Define Python user package site
execute_process(COMMAND python -m site --user-site OUTPUT_VARIABLE Python_UserSitePackage OUTPUT_STRIP_TRAILING_WHITESPACE)

# Install module to the user package site
add_custom_command(TARGET callback POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
    $<TARGET_FILE:callback>
    "${Python_UserSitePackage}/$<TARGET_FILE_NAME:callback>"
)

# Generate Python interface file in the user package site
add_custom_command(TARGET callback POST_BUILD
     COMMAND stubgen --include-docstrings -o ${Python_UserSitePackage} -m $<TARGET_FILE_BASE_NAME:callback>
     WORKING_DIRECTORY $<TARGET_FILE_DIR:callback>
 )
  