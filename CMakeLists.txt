cmake_minimum_required(VERSION 3.16)
project(HizliHaberlesme)

find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

# 1. Adımda yazdığımız Mesaj.idl dosyasından C++ kodları üret
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Mesaj.cxx
           ${CMAKE_CURRENT_SOURCE_DIR}/MesajPubSubTypes.cxx
           ${CMAKE_CURRENT_SOURCE_DIR}/Mesaj.h
           ${CMAKE_CURRENT_SOURCE_DIR}/MesajPubSubTypes.h
    COMMAND fastddsgen -replace -d ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Mesaj.idl
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Mesaj.idl
)

add_library(MesajLib ${CMAKE_CURRENT_SOURCE_DIR}/Mesaj.cxx ${CMAKE_CURRENT_SOURCE_DIR}/MesajPubSubTypes.cxx)
target_link_libraries(MesajLib fastrtps fastcdr)

# Konuşan Adamı inşa et
add_executable(konusan_adam Yayinici.cpp)
target_link_libraries(konusan_adam MesajLib fastrtps fastcdr)

# Dinleyen Adamı inşa et
add_executable(dinleyen_adam Abone.cpp)
target_link_libraries(dinleyen_adam MesajLib fastrtps fastcdr)