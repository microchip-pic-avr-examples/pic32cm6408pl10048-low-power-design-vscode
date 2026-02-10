set(DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB2_default_ONoLORFW "c:/Program Files/Microchip/xc32/v5.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB2_default_ONoLORFW ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB2/default.elf)
set(DEPENDENT_TARGET_DIRPL10_LowPower_LAB2_default_ONoLORFW ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB2)
set(DEPENDENT_BYPRODUCTSPL10_LowPower_LAB2_default_ONoLORFW ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB2_default_ONoLORFW}/${sourceFileNamePL10_LowPower_LAB2_default_ONoLORFW}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB2_default_ONoLORFW}/${sourceFileNamePL10_LowPower_LAB2_default_ONoLORFW}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB2_default_ONoLORFW} --image ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB2_default_ONoLORFW} --image-generated-c ${sourceFileNamePL10_LowPower_LAB2_default_ONoLORFW}.c --image-generated-h ${sourceFileNamePL10_LowPower_LAB2_default_ONoLORFW}.h --image-copy-mode ${modePL10_LowPower_LAB2_default_ONoLORFW} --image-offset ${addressPL10_LowPower_LAB2_default_ONoLORFW} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB2_default_ONoLORFW}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB2_default_ONoLORFW})
add_custom_target(
    dependent_produced_source_artifactPL10_LowPower_LAB2_default_ONoLORFW 
    DEPENDS ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB2_default_ONoLORFW}/${sourceFileNamePL10_LowPower_LAB2_default_ONoLORFW}.c
    )
