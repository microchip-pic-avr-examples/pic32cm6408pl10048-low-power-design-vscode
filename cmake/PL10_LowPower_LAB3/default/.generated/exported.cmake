set(DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB3_default__d3Xmxts "c:/Program Files/Microchip/xc32/v5.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB3_default__d3Xmxts ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB3/default.elf)
set(DEPENDENT_TARGET_DIRPL10_LowPower_LAB3_default__d3Xmxts ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB3)
set(DEPENDENT_BYPRODUCTSPL10_LowPower_LAB3_default__d3Xmxts ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB3_default__d3Xmxts}/${sourceFileNamePL10_LowPower_LAB3_default__d3Xmxts}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB3_default__d3Xmxts}/${sourceFileNamePL10_LowPower_LAB3_default__d3Xmxts}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB3_default__d3Xmxts} --image ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB3_default__d3Xmxts} --image-generated-c ${sourceFileNamePL10_LowPower_LAB3_default__d3Xmxts}.c --image-generated-h ${sourceFileNamePL10_LowPower_LAB3_default__d3Xmxts}.h --image-copy-mode ${modePL10_LowPower_LAB3_default__d3Xmxts} --image-offset ${addressPL10_LowPower_LAB3_default__d3Xmxts} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB3_default__d3Xmxts}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB3_default__d3Xmxts})
add_custom_target(
    dependent_produced_source_artifactPL10_LowPower_LAB3_default__d3Xmxts 
    DEPENDS ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB3_default__d3Xmxts}/${sourceFileNamePL10_LowPower_LAB3_default__d3Xmxts}.c
    )
