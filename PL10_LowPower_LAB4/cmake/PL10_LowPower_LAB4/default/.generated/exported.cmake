set(DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB4_default_hsY_pi_g "c:/Program Files/Microchip/xc32/v5.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB4_default_hsY_pi_g ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB4/default.elf)
set(DEPENDENT_TARGET_DIRPL10_LowPower_LAB4_default_hsY_pi_g ${CMAKE_CURRENT_LIST_DIR}/../../../../out/PL10_LowPower_LAB4)
set(DEPENDENT_BYPRODUCTSPL10_LowPower_LAB4_default_hsY_pi_g ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB4_default_hsY_pi_g}/${sourceFileNamePL10_LowPower_LAB4_default_hsY_pi_g}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB4_default_hsY_pi_g}/${sourceFileNamePL10_LowPower_LAB4_default_hsY_pi_g}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXPL10_LowPower_LAB4_default_hsY_pi_g} --image ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB4_default_hsY_pi_g} --image-generated-c ${sourceFileNamePL10_LowPower_LAB4_default_hsY_pi_g}.c --image-generated-h ${sourceFileNamePL10_LowPower_LAB4_default_hsY_pi_g}.h --image-copy-mode ${modePL10_LowPower_LAB4_default_hsY_pi_g} --image-offset ${addressPL10_LowPower_LAB4_default_hsY_pi_g} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB4_default_hsY_pi_g}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFPL10_LowPower_LAB4_default_hsY_pi_g})
add_custom_target(
    dependent_produced_source_artifactPL10_LowPower_LAB4_default_hsY_pi_g 
    DEPENDS ${DEPENDENT_TARGET_DIRPL10_LowPower_LAB4_default_hsY_pi_g}/${sourceFileNamePL10_LowPower_LAB4_default_hsY_pi_g}.c
    )
