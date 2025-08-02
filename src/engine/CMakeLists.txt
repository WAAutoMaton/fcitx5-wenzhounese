set(WENZHOUNESE_SOURCES
    engine.cpp
)
add_fcitx5_addon(wenzhounese ${WENZHOUNESE_SOURCES})
target_link_libraries(wenzhounese Fcitx5::Core Fcitx5::Config Fcitx5::Module::Spell Fcitx5::Module::Punctuation Fcitx5::Module::QuickPhrase LibIME::Wenzhounese)
target_compile_definitions(wenzhounese
    PRIVATE LIBIME_WENZHOUNESE_INSTALL_PKGDATADIR=\"${LIBIME_WENZHOUNESE_INSTALL_PKGDATADIR}\"
            FCITX_GETTEXT_DOMAIN=\"fcitx5-wenzhounese\"
)
fcitx5_add_i18n_definition(TARGETS wenzhounese)

install(TARGETS wenzhounese DESTINATION "${CMAKE_INSTALL_LIBDIR}/fcitx5")
fcitx5_translate_desktop_file(wenzhounese.conf.in wenzhounese.conf)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/wenzhounese.conf" DESTINATION "${CMAKE_INSTALL_DATADIR}/fcitx5/inputmethod" COMPONENT config)
configure_file(wenzhounese-addon.conf.in.in ${CMAKE_CURRENT_BINARY_DIR}/wenzhounese-addon.conf.in @ONLY)
fcitx5_translate_desktop_file(${CMAKE_CURRENT_BINARY_DIR}/wenzhounese-addon.conf.in wenzhounese-addon.conf)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/wenzhounese-addon.conf" RENAME wenzhounese.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/addon" COMPONENT config)


fcitx5_translate_desktop_file(org.fcitx.Fcitx5.Addon.Wenzhounese.metainfo.xml.in
                              org.fcitx.Fcitx5.Addon.Wenzhounese.metainfo.xml XML)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/org.fcitx.Fcitx5.Addon.Wenzhounese.metainfo.xml" DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
