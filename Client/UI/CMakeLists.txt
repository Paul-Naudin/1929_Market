# UI library
add_library(UI
    Command.cpp
    CommandFactory.cpp
)
target_link_libraries(UI PUBLIC FIXProtocol)
target_include_directories(UI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
