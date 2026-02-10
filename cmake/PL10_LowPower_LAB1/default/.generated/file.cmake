# The following variables contains the files used by the different stages of the build process.
set(PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_assemble)
set_source_files_properties(${PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_assembleWithPreprocess)
set_source_files_properties(${PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_assembleWithPreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_assembleWithPreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/exceptions.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/initialization.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/interrupts.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/libc_syscalls.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/peripheral/clock/plib_clock.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/peripheral/evsys/plib_evsys.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/peripheral/nvic/plib_nvic.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/peripheral/pm/plib_pm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/peripheral/port/plib_port.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/peripheral/supc/plib_supc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/startup_xc32.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/stdio/xc32_monitor.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/lab1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/exceptions.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/initialization.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/interrupts.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/libc_syscalls.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/peripheral/adc/plib_adc0.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/peripheral/clock/plib_clock.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/peripheral/evsys/plib_evsys.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/peripheral/nvic/plib_nvic.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/peripheral/nvmctrl/plib_nvmctrl.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/peripheral/pm/plib_pm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/peripheral/port/plib_port.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/peripheral/rtc/plib_rtc_timer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/peripheral/sercom/usart/plib_sercom1_usart.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/peripheral/supc/plib_supc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/peripheral/tc/plib_tc1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/startup_xc32.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/config/default/stdio/xc32_monitor.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/lab2.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB2/My_MCC_Config_LAB2/src/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/exceptions.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/initialization.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/interrupts.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/libc_syscalls.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/peripheral/adc/plib_adc0.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/peripheral/clock/plib_clock.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/peripheral/evsys/plib_evsys.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/peripheral/nvic/plib_nvic.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/peripheral/nvmctrl/plib_nvmctrl.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/peripheral/pm/plib_pm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/peripheral/port/plib_port.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/peripheral/rtc/plib_rtc_timer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/peripheral/sercom/usart/plib_sercom1_usart.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/peripheral/supc/plib_supc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/peripheral/tc/plib_tc1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/startup_xc32.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/config/default/stdio/xc32_monitor.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/lab3.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB3/My_MCC_Config_LAB3/src/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/exceptions.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/initialization.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/interrupts.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/libc_syscalls.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/adc/plib_adc0.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/clock/plib_clock.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/dmac/plib_dmac.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/evsys/plib_evsys.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/nvic/plib_nvic.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/nvmctrl/plib_nvmctrl.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/pm/plib_pm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/port/plib_port.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/rtc/plib_rtc_timer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/sercom/usart/plib_sercom1_usart.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/supc/plib_supc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/tc/plib_tc0.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/tc/plib_tc1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/peripheral/tc/plib_tc2.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/startup_xc32.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/config/default/stdio/xc32_monitor.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/lab4.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB4/My_MCC_Config_LAB4/src/main.c")
set_source_files_properties(${PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_compile_cpp)
set_source_files_properties(${PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_compile_cpp} PROPERTIES LANGUAGE CXX)
set(PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_link)
set(PL10_LowPower_LAB1_default_default_XC32_FILE_TYPE_bin2hex)

# The linker script used for the build.
set(PL10_LowPower_LAB1_default_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/../../../PL10_LowPower_LAB1/My_MCC_Config_LAB1/src/config/default/PIC32CM6408PL10048.ld")
set(PL10_LowPower_LAB1_default_image_name "default.elf")
set(PL10_LowPower_LAB1_default_image_base_name "default")

# The output directory of the final image.
set(PL10_LowPower_LAB1_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/PL10_LowPower_LAB1")

# The full path to the final image.
set(PL10_LowPower_LAB1_default_full_path_to_image ${PL10_LowPower_LAB1_default_output_dir}/${PL10_LowPower_LAB1_default_image_name})
