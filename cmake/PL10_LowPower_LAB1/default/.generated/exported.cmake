set(DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB1_default_M7H6pEmR "c:/Program Files/Microchip/xc32/v5.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB1_default_M7H6pEmR ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB1/default.elf)
set(DEPENDENT_TARGET_DIRPL10_LowPower_LAB1_default_M7H6pEmR ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB1)
set(DEPENDENT_BYPRODUCTSPL10_LowPower_LAB1_default_M7H6pEmR ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB1_default_M7H6pEmR}/${sourceFileNamePL10_LowPower_LAB1_default_M7H6pEmR}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB1_default_M7H6pEmR}/${sourceFileNamePL10_LowPower_LAB1_default_M7H6pEmR}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB1_default_M7H6pEmR} --image ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB1_default_M7H6pEmR} --image-generated-c ${sourceFileNamePL10_LowPower_LAB1_default_M7H6pEmR}.c --image-generated-h ${sourceFileNamePL10_LowPower_LAB1_default_M7H6pEmR}.h --image-copy-mode ${modePL10_LowPower_LAB1_default_M7H6pEmR} --image-offset ${addressPL10_LowPower_LAB1_default_M7H6pEmR} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB1_default_M7H6pEmR}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB1_default_M7H6pEmR})
add_custom_target(
    dependent_produced_source_artifactPL10_LowPower_LAB1_default_M7H6pEmR 
    DEPENDS ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB1_default_M7H6pEmR}/${sourceFileNamePL10_LowPower_LAB1_default_M7H6pEmR}.c
    )
