
add_executable(libime_wenzhounesedict libime_wenzhounesedict.cpp)
target_link_libraries(libime_wenzhounesedict LibIME::Wenzhounese)
install(TARGETS libime_wenzhounesedict DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT tools)
add_executable(LibIME::wenzhounesedict ALIAS libime_wenzhounesedict)
