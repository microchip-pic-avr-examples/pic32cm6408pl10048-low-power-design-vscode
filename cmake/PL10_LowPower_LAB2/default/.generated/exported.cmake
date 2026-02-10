set(DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB2_default_iUfvlg9A "c:/Program Files/Microchip/xc32/v5.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB2_default_iUfvlg9A ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB2/default.elf)
set(DEPENDENT_TARGET_DIRPL10_LowPower_LAB2_default_iUfvlg9A ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB2)
set(DEPENDENT_BYPRODUCTSPL10_LowPower_LAB2_default_iUfvlg9A ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB2_default_iUfvlg9A}/${sourceFileNamePL10_LowPower_LAB2_default_iUfvlg9A}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB2_default_iUfvlg9A}/${sourceFileNamePL10_LowPower_LAB2_default_iUfvlg9A}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB2_default_iUfvlg9A} --image ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB2_default_iUfvlg9A} --image-generated-c ${sourceFileNamePL10_LowPower_LAB2_default_iUfvlg9A}.c --image-generated-h ${sourceFileNamePL10_LowPower_LAB2_default_iUfvlg9A}.h --image-copy-mode ${modePL10_LowPower_LAB2_default_iUfvlg9A} --image-offset ${addressPL10_LowPower_LAB2_default_iUfvlg9A} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB2_default_iUfvlg9A}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB2_default_iUfvlg9A})
add_custom_target(
    dependent_produced_source_artifactPL10_LowPower_LAB2_default_iUfvlg9A 
    DEPENDS ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB2_default_iUfvlg9A}/${sourceFileNamePL10_LowPower_LAB2_default_iUfvlg9A}.c
    )
