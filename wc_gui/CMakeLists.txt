set(MODULE "wc_gui")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS Widgets Core Gui Qml Multimedia MultimediaWidgets REQUIRED)

file(GLOB SOURCES ./*.cpp ./*.h ./*.c ./*.ui)

add_executable(${MODULE} ${SOURCES})
if(WIN32)
	target_link_libraries(${MODULE} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Qml Qt5::Multimedia Qt5::MultimediaWidgets ${CMAKE_PROJECT_NAME}_shared ${LINK_LIB_LIST})
else()
        target_link_libraries(${MODULE} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Qml Qt5::Multimedia Qt5::MultimediaWidgets ${CMAKE_PROJECT_NAME}_shared ${LINK_LIB_LIST} pthread)
endif()

