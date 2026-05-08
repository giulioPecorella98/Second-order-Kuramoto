cmake_minimum_required(VERSION 3.10)

project(Second-order-Kuramoto)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

include_directories(simulation_engine/include)

set(SOURCES_one
    simulation_engine/src/engine_one.cpp
    simulation_engine/src/saving/saving_density.cpp
    simulation_engine/src/numerical_scheme/finite_difference.cpp
    simulation_engine/src/simulation_parameters/initial_conditions.cpp
    simulation_engine/src/simulation_parameters/parameters_one.cpp
    simulation_engine/src/numerical_scheme/order_parameter.cpp
    simulation_engine/src/numerical_scheme/frequency_order_parameter.cpp
)
#[[
set(SOURCES_two
    simulation_engine/src/engine_two.cpp
    simulation_engine/src/saving/saving_r.cpp
    simulation_engine/src/numerical_scheme/finite_difference.cpp
    simulation_engine/src/simulation_parameters/initial_conditions.cpp
    simulation_engine/src/simulation_parameters/parameters_two.cpp
    simulation_engine/src/numerical_scheme/order_parameter.cpp
)]]

add_compile_definitions(PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

add_executable(engine_one ${SOURCES_one})
#[[
add_executable(engine_two ${SOURCES_two})]]
#[[
set_target_properties(engine_one engine_two PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/simulation_engine
)]]



set_target_properties(engine_one PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/simulation_engine
)


target_link_libraries(engine_one PRIVATE m)
#[[target_link_libraries(engine_two PRIVATE m)]]