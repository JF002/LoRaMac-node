project(Pinecone)
cmake_minimum_required(VERSION 3.6)

list(APPEND ${PROJECT_NAME}_SOURCES

    ${CMAKE_CURRENT_SOURCE_DIR}/littlefs/lfs_util.c
    ${CMAKE_CURRENT_SOURCE_DIR}/littlefs/lfs.c

    board.c
    sx1262-board.c
    "${CMAKE_CURRENT_SOURCE_DIR}/../mcu/utilities.c"
    delay-board.c
    gpio-board.c
    spi-board.c
    rtc-board.c
    eeprom-board.c
    )

add_definitions(-DLFS_NO_DEBUG)
add_definitions(-DLFS_NO_ERROR)
add_definitions(-DLFS_NO_WARN)
add_definitions(-DLFS_NO_ASSERT)
#add_definitions(-DLFS_NO_MALLOC)

add_library(${PROJECT_NAME} OBJECT EXCLUDE_FROM_ALL ${${PROJECT_NAME}_SOURCES})
target_link_options(${PROJECT_NAME} INTERFACE -u_malloc_r -u_vsnprintf)

target_link_libraries(${PROJECT_NAME} PRIVATE  sdk_intf_lib)

target_link_libraries(${PROJECT_NAME} PRIVATE -Wl,--start-group lhal bl602 utils app -Wl,--end-group)


target_include_directories(${PROJECT_NAME} PUBLIC
    $<TARGET_PROPERTY:board,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:system,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:radio,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:peripherals,INTERFACE_INCLUDE_DIRECTORIES>
    )
