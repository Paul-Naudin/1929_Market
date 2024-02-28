# Set up Qt5
set(Qt5_DIR /home/paul/Qt/5.15.2/gcc_64/lib/cmake/Qt5) # Replace with your Qt5 directory
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Client executable
add_executable(Client
    main.cpp
)
target_link_libraries(Client PRIVATE FIXProtocol CommandLine Qt5::Core Qt5::Gui Qt5::Widgets)

# Command line library
add_library(CommandLine
    GUI/Command.cpp
    GUI/CommandFactory.cpp
    GUI/Client.cpp
)
target_link_libraries(CommandLine PRIVATE FIXProtocol)

# MVC library
add_subdirectory(MVC)