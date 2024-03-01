# UI Library
add_subdirectory(UI)

# Client executable
add_executable(Client
    Client.cpp
    main.cpp
)
target_link_libraries(Client PRIVATE UI)
target_include_directories(Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/UI)