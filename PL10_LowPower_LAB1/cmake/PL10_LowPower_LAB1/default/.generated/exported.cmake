set(DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB1_default_XhjKVWxN "c:/Program Files/Microchip/xc32/v5.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB1_default_XhjKVWxN ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB1/default.elf)
set(DEPENDENT_TARGET_DIRPL10_LowPower_LAB1_default_XhjKVWxN ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB1)
set(DEPENDENT_BYPRODUCTSPL10_LowPower_LAB1_default_XhjKVWxN ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB1_default_XhjKVWxN}/${sourceFileNamePL10_LowPower_LAB1_default_XhjKVWxN}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB1_default_XhjKVWxN}/${sourceFileNamePL10_LowPower_LAB1_default_XhjKVWxN}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB1_default_XhjKVWxN} --image ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB1_default_XhjKVWxN} --image-generated-c ${sourceFileNamePL10_LowPower_LAB1_default_XhjKVWxN}.c --image-generated-h ${sourceFileNamePL10_LowPower_LAB1_default_XhjKVWxN}.h --image-copy-mode ${modePL10_LowPower_LAB1_default_XhjKVWxN} --image-offset ${addressPL10_LowPower_LAB1_default_XhjKVWxN} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB1_default_XhjKVWxN}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB1_default_XhjKVWxN})
add_custom_target(
    dependent_produced_source_artifactPL10_LowPower_LAB1_default_XhjKVWxN 
    DEPENDS ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB1_default_XhjKVWxN}/${sourceFileNamePL10_LowPower_LAB1_default_XhjKVWxN}.c
    )
