set(DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB3_Program__AQTAt62 "c:/Program Files/Microchip/xc32/v5.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB3_Program__AQTAt62 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB3/Program.elf)
set(DEPENDENT_TARGET_DIRPL10_LowPower_LAB3_Program__AQTAt62 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB3)
set(DEPENDENT_BYPRODUCTSPL10_LowPower_LAB3_Program__AQTAt62 ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB3_Program__AQTAt62}/${sourceFileNamePL10_LowPower_LAB3_Program__AQTAt62}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB3_Program__AQTAt62}/${sourceFileNamePL10_LowPower_LAB3_Program__AQTAt62}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB3_Program__AQTAt62} --image ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB3_Program__AQTAt62} --image-generated-c ${sourceFileNamePL10_LowPower_LAB3_Program__AQTAt62}.c --image-generated-h ${sourceFileNamePL10_LowPower_LAB3_Program__AQTAt62}.h --image-copy-mode ${modePL10_LowPower_LAB3_Program__AQTAt62} --image-offset ${addressPL10_LowPower_LAB3_Program__AQTAt62} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB3_Program__AQTAt62}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB3_Program__AQTAt62})
add_custom_target(
    dependent_produced_source_artifactPL10_LowPower_LAB3_Program__AQTAt62 
    DEPENDS ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB3_Program__AQTAt62}/${sourceFileNamePL10_LowPower_LAB3_Program__AQTAt62}.c
    )
