# Client executable
add_executable(Client
    main.cpp
)
target_link_libraries(Client PRIVATE FIXProtocol CommandLine)

# Command line library
add_library(CommandLine
    GUI/Command.cpp
    GUI/CommandFactory.cpp
    GUI/Client.cpp
)
target_link_libraries(CommandLine PRIVATE FIXProtocol)