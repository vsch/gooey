cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_TOOLCHAIN_FILE /Users/vlad/src/projects/CLionArduinoPlugin/src/com/vladsch/clionarduinoplugin/resources/arduino-cmake/cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 98)
set(PROJECT_NAME gooey)

# use UART with interrupts if set to non-zero
set(UART_INT 0)
add_compile_definitions(INCLUDE_TWI_INT)

# use DEBUG_MODE dev version of options
set(DEBUG_MODE 1)
IF (DEBUG_MODE)
    set(ARDUINO_CPU 16MHzatmega328)
    add_compile_definitions(DEBUG_MODE)
ELSE ()
    set(ARDUINO_CPU 8MHzatmega328)
ENDIF ()

set(${PROJECT_NAME}_BOARD pro)
project(${PROJECT_NAME})

# Define additional source and header files or default arduino sketch files
set(${PROJECT_NAME}_SRCS
        gooey_test.cpp
        power_down.c
        src/ButtonHandler.cpp
        src/FieldEditor.cpp
        src/InterfaceManager.cpp
        src/Popup.cpp
        src/PopupForm.cpp
        src/PopupMenu.cpp
        src/PopupMessage.cpp
        src/PopupOptionsMenu.cpp
        src/Utils.c
        )
set(${PROJECT_NAME}_HDRS
        power_down.h
        config.h
        pin_config.h
        type_defs.h
        src/gooey.h
        src/ButtonHandler.h
        src/Events.h
        src/FieldEditor.h
        src/InterfaceHandler.h
        src/InterfaceManager.h
        src/Popup.h
        src/PopupForm.h
        src/PopupMenu.h
        src/PopupMessage.h
        src/PopupOptionsMenu.h
        src/Utils.h
        )

### Additional static libraries to include in the target.
#set(${PROJECT_NAME}_LIBS/..)
### additional library directories to search
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

# For nested library sources replace ${LIB_NAME} with library name for each library
# set(_RECURSE true)

#set(${CMAKE_PROJECT_NAME}_SKETCH gooey_test.ino)

#### Additional settings for programmer. From programmers.txt
set(${PROJECT_NAME}_PROGRAMMER avrispmkii)
set(${PROJECT_NAME}_PORT /dev/cu.usbserial-00000000)
set(pro.upload.speed 57600)

## Verbose build process
set(${PROJECT_NAME}_AFLAGS -v)

generate_arduino_firmware(${PROJECT_NAME})

print_board_list()
print_programmer_list()

