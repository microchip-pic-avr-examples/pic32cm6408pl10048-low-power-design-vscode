# The following functions contains all the flags passed to the different build stages.

set(PACK_REPO_PATH "C:/Users/C76801/.mchp_packs" CACHE PATH "Path to the root of a pack repository.")

function(PL10_LowPower_LAB3_Program_Program_toolchain_assemble_rule target)
    set(options
        "-g"
        "${ASSEMBLER_PRE}"
        "-mprocessor=32CM6408PL10048"
        "-Wa,--defsym=__MPLAB_BUILD=1${MP_EXTRA_AS_POST},--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--gdwarf-2"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CM-PL_DFP/1.2.325")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target} PRIVATE "__DEBUG=1")
endfunction()
function(PL10_LowPower_LAB3_Program_Program_toolchain_assembleWithPreprocess_rule target)
    set(options
        "-x"
        "assembler-with-cpp"
        "-g"
        "${MP_EXTRA_AS_PRE}"
        "${DEBUGGER_NAME_AS_MACRO}"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CM-PL_DFP/1.2.325"
        "-mprocessor=32CM6408PL10048"
        "-Wa,--defsym=__MPLAB_BUILD=1${MP_EXTRA_AS_POST},--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=1"
        PRIVATE "XPRJ_Program=Program")
endfunction()
function(PL10_LowPower_LAB3_Program_Program_toolchain_compile_rule target)
    set(options
        "-g"
        "${CC_PRE}"
        "-x"
        "c"
        "-c"
        "-mprocessor=32CM6408PL10048"
        "-ffunction-sections"
        "-fdata-sections"
        "-O1"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CM-PL_DFP/1.2.325")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG"
        PRIVATE "XPRJ_Program=Program")
    target_include_directories(${target}
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/config/default"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/CMSIS"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/CMSIS/CMSIS/Core/Include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/PIC32CM6408PL10048_DFP"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/config/default"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/CMSIS"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/CMSIS/CMSIS/Core/Include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/PIC32CM6408PL10048_DFP"
        PRIVATE "${PACK_REPO_PATH}/ARM/CMSIS/6.2.0/CMSIS/Core/Include")
endfunction()
function(PL10_LowPower_LAB3_Program_Program_toolchain_compile_cpp_rule target)
    set(options
        "-g"
        "${CC_PRE}"
        "${DEBUGGER_NAME_AS_MACRO}"
        "-mprocessor=32CM6408PL10048"
        "-frtti"
        "-fexceptions"
        "-fno-check-new"
        "-fenforce-eh-specs"
        "-ffunction-sections"
        "-O1"
        "-fno-common"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CM-PL_DFP/1.2.325")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG"
        PRIVATE "XPRJ_Program=Program")
    target_include_directories(${target}
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/config/default"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/CMSIS"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/CMSIS/CMSIS/Core/Include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/PIC32CM6408PL10048_DFP"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/config/default"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/CMSIS"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/CMSIS/CMSIS/Core/Include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../My_MCC_Config/src/packs/PIC32CM6408PL10048_DFP"
        PRIVATE "${PACK_REPO_PATH}/ARM/CMSIS/6.2.0/CMSIS/Core/Include")
endfunction()
function(PL10_LowPower_LAB3_Program_dependentObject_rule target)
    set(options
        "-mprocessor=32CM6408PL10048"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CM-PL_DFP/1.2.325")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
endfunction()
function(PL10_LowPower_LAB3_Program_link_rule target)
    set(options
        "-g"
        "${MP_EXTRA_LD_PRE}"
        "${DEBUGGER_OPTION_TO_LINKER}"
        "${DEBUGGER_NAME_AS_MACRO}"
        "-mprocessor=32CM6408PL10048"
        "-mno-device-startup-code"
        "-Wl,--defsym=__MPLAB_BUILD=1${MP_EXTRA_LD_POST},--script=${PL10_LowPower_LAB3_Program_LINKER_SCRIPT},--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=_min_heap_size=0,--gc-sections,-Map=mem.map,--report-mem,--memorysummary,memoryfile.xml"
        "-mdfp=${PACK_REPO_PATH}/Microchip/PIC32CM-PL_DFP/1.2.325")
    list(REMOVE_ITEM options "")
    target_link_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target} PRIVATE "XPRJ_Program=Program")
endfunction()
function(PL10_LowPower_LAB3_Program_bin2hex_rule target)
    add_custom_target(
        PL10_LowPower_LAB3_Program_Bin2Hex ALL
        COMMAND ${MP_BIN2HEX} ${PL10_LowPower_LAB3_Program_image_name}
        WORKING_DIRECTORY ${PL10_LowPower_LAB3_Program_output_dir}
        BYPRODUCTS "${PL10_LowPower_LAB3_Program_output_dir}/${PL10_LowPower_LAB3_Program_image_base_name}.hex"
        COMMENT "Convert build file to .hex")
    add_dependencies(PL10_LowPower_LAB3_Program_Bin2Hex ${target})
endfunction()
