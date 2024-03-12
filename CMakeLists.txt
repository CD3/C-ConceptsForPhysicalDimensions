cmake_minimum_required(VERSION 3.20)

find_package(BoostUnitDefinitions)
find_package(Boost)
add_executable(quantity_concepts ./quantity_concepts.cpp)
set_property(TARGET quantity_concepts PROPERTY CXX_STANDARD 20)
target_link_libraries(quantity_concepts PUBLIC BoostUnitDefinitions::BoostUnitDefinitions Boost::headers)
