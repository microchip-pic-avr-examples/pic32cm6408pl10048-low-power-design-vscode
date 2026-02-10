set(DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB4_default__jaVYRpt "c:/Program Files/Microchip/xc32/v5.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB4_default__jaVYRpt ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB4/default.elf)
set(DEPENDENT_TARGET_DIRPL10_LowPower_LAB4_default__jaVYRpt ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB4)
set(DEPENDENT_BYPRODUCTSPL10_LowPower_LAB4_default__jaVYRpt ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB4_default__jaVYRpt}/${sourceFileNamePL10_LowPower_LAB4_default__jaVYRpt}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB4_default__jaVYRpt}/${sourceFileNamePL10_LowPower_LAB4_default__jaVYRpt}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB4_default__jaVYRpt} --image ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB4_default__jaVYRpt} --image-generated-c ${sourceFileNamePL10_LowPower_LAB4_default__jaVYRpt}.c --image-generated-h ${sourceFileNamePL10_LowPower_LAB4_default__jaVYRpt}.h --image-copy-mode ${modePL10_LowPower_LAB4_default__jaVYRpt} --image-offset ${addressPL10_LowPower_LAB4_default__jaVYRpt} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB4_default__jaVYRpt}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB4_default__jaVYRpt})
add_custom_target(
    dependent_produced_source_artifactPL10_LowPower_LAB4_default__jaVYRpt 
    DEPENDS ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB4_default__jaVYRpt}/${sourceFileNamePL10_LowPower_LAB4_default__jaVYRpt}.c
    )
