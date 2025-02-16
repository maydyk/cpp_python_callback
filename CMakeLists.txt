# version 3.11 or later of CMake or needed later for installing GoogleTest
# so let's require it now.
cmake_minimum_required(VERSION 3.11-3.18)

project(callbask-test)

# Debug options
add_compile_options(-g -O0)

# Setup Python
find_package(Python3 REQUIRED COMPONENTS Development)
find_package(Boost REQUIRED COMPONENTS python3)


add_library(callback MODULE callback_module.cpp)
set_target_properties(callback PROPERTIES
    PREFIX "${PYTHON_MODULE_PREFIX}"
)
 
# Setup file extensiton for Windows
if ($WIN32)
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


