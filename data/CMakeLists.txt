set(LM_SRC "${CMAKE_CURRENT_SOURCE_DIR}/wenzhounese.arpa")
set(LM_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/wenzhounese.lm")
set(LM_PREDICT_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/wenzhounese.lm.predict")

add_custom_command(
  OUTPUT "${LM_OUTPUT}"
  DEPENDS "${LM_SRC}" LibIME::slm_build_binary
  COMMAND LibIME::slm_build_binary -s -a 22 -q 8 trie "${LM_SRC}" "${LM_OUTPUT}")
add_custom_target(model ALL DEPENDS "${LM_OUTPUT}")

add_custom_command(
  OUTPUT "${LM_PREDICT_OUTPUT}"
  DEPENDS "${LM_SRC}" model LibIME::prediction
  COMMAND LibIME::prediction "${LM_OUTPUT}" "${LM_SRC}" "${LM_PREDICT_OUTPUT}")
add_custom_target(predict ALL DEPENDS "${LM_PREDICT_OUTPUT}")

set(DICT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/words.txt")
set(DICT_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/wenzhounese.dict")
add_custom_command(
  OUTPUT "${DICT_OUTPUT}"
  DEPENDS "${DICT_SRC}" libime_wenzhounesedict
  COMMAND $<TARGET_FILE:libime_wenzhounesedict> "${DICT_SRC}" "${DICT_OUTPUT}")
add_custom_target(dict ALL DEPENDS "${DICT_OUTPUT}")
install(FILES "${DICT_OUTPUT}" DESTINATION "${LIBIME_WENZHOUNESE_INSTALL_PKGDATADIR}")
install(FILES "${LM_OUTPUT}" "${LM_PREDICT_OUTPUT}" DESTINATION "${LIBIME_WENZHOUNESE_INSTALL_LIBDATADIR}")
