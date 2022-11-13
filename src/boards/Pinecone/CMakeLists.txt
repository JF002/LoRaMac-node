project(Pinecone)
cmake_minimum_required(VERSION 3.6)

list(APPEND ${PROJECT_NAME}_SOURCES
    board.c
    sx1262-board.c
    "${CMAKE_CURRENT_SOURCE_DIR}/../mcu/utilities.c"
    delay-board.c
    gpio-board.c
    spi-board.c
    rtc-board.c
    eeprom-board.c
    )

add_library(${PROJECT_NAME} OBJECT EXCLUDE_FROM_ALL ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE  sdk_intf_lib)
target_link_libraries(${PROJECT_NAME} PRIVATE -Wl,--start-group lhal bl602 utils app -Wl,--end-group)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<TARGET_PROPERTY:board,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:system,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:radio,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:peripherals,INTERFACE_INCLUDE_DIRECTORIES>
    )
