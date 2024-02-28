# FIX Protocol library
add_library(MVC
    UserController.cpp
    UserModel.cpp
    UserView.cpp
)
target_include_directories(MVC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Interfaces)
target_link_libraries(MVC PRIVATE FIXProtocol Qt5::Core Qt5::Gui Qt5::Widgets)
