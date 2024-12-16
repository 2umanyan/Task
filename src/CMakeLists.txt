add_executable(task Client.cpp Club.cpp Event.cpp main.cpp)

target_include_directories(task PRIVATE ${gtest_SOURCE_DIR}/include)

install(TARGETS task DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin)