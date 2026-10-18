ecm_setup_version(PROJECT
                  VARIABLE_PREFIX IMEWenzhounese
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/LibIMEWenzhouneseConfigVersion.cmake"
                  SOVERSION 1)

set(LIBIME_WENZHOUNESE_HDRS
    wenzhounesedictionary.h
    wenzhouneseencoder.h
    wenzhounesedata.h
    wenzhounesecontext.h
    wenzhounesedecoder.h
    wenzhouneseime.h
    wenzhounesematchstate.h
    ${CMAKE_CURRENT_BINARY_DIR}/libimewenzhounese_export.h
    )

set(LIBIME_WENZHOUNESE_SRCS
    wenzhouneseencoder.cpp
    wenzhounesedata.cpp
    wenzhounesedictionary.cpp
    wenzhounesecontext.cpp
    wenzhounesedecoder.cpp
    wenzhouneseime.cpp
    wenzhounesematchstate.cpp
    )

add_library(IMEWenzhounese SHARED ${LIBIME_WENZHOUNESE_SRCS})
set_target_properties(IMEWenzhounese
                      PROPERTIES VERSION ${IMEWenzhounese_VERSION}
                      SOVERSION ${IMEWenzhounese_SOVERSION}
                      EXPORT_NAME Wenzhounese
)
target_include_directories(IMEWenzhounese PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}/LibIME>)

target_link_libraries(IMEWenzhounese PUBLIC Fcitx5::Utils Boost::boost LibIME::Core PRIVATE Boost::iostreams PkgConfig::ZSTD)

install(TARGETS IMEWenzhounese EXPORT LibIMEWenzhouneseTargets
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT lib
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT Devel)
install(FILES ${LIBIME_WENZHOUNESE_HDRS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/LibIME/libime/wenzhounese" COMPONENT header)

add_library(LibIME::Wenzhounese ALIAS IMEWenzhounese)

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/LibIMEWenzhouneseConfig.cmake.in"
                              "${CMAKE_CURRENT_BINARY_DIR}/LibIMEWenzhouneseConfig.cmake"
                              INSTALL_DESTINATION  "${CMAKE_INSTALL_LIBDIR}/cmake/LibIMEWenzhounese"
)

generate_export_header(IMEWenzhounese BASE_NAME LibIMEWenzhounese)

install(EXPORT LibIMEWenzhouneseTargets
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/LibIMEWenzhounese"
        FILE LibIMEWenzhouneseTargets.cmake
        NAMESPACE LibIME::
        COMPONENT Devel)

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/LibIMEWenzhouneseConfig.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/LibIMEWenzhouneseConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/LibIMEWenzhounese"
        COMPONENT Devel)
