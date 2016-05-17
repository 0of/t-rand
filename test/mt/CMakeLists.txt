find_package(Threads REQUIRED)

file(GLOB tests "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(mt ${tests})
target_link_libraries(mt ${CMAKE_THREAD_LIBS_INIT})