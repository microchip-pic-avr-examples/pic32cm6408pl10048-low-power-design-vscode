include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(PL10_LowPower_LAB3_default_library_list )

# Handle files with suffix s, for group default-XC32
if(PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_assemble)
add_library(PL10_LowPower_LAB3_default_default_XC32_assemble OBJECT ${PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_assemble})
    PL10_LowPower_LAB3_default_default_XC32_assemble_rule(PL10_LowPower_LAB3_default_default_XC32_assemble)
    list(APPEND PL10_LowPower_LAB3_default_library_list "$<TARGET_OBJECTS:PL10_LowPower_LAB3_default_default_XC32_assemble>")

endif()

# Handle files with suffix S, for group default-XC32
if(PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_assembleWithPreprocess)
add_library(PL10_LowPower_LAB3_default_default_XC32_assembleWithPreprocess OBJECT ${PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_assembleWithPreprocess})
    PL10_LowPower_LAB3_default_default_XC32_assembleWithPreprocess_rule(PL10_LowPower_LAB3_default_default_XC32_assembleWithPreprocess)
    list(APPEND PL10_LowPower_LAB3_default_library_list "$<TARGET_OBJECTS:PL10_LowPower_LAB3_default_default_XC32_assembleWithPreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC32
if(PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_compile)
add_library(PL10_LowPower_LAB3_default_default_XC32_compile OBJECT ${PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_compile})
    PL10_LowPower_LAB3_default_default_XC32_compile_rule(PL10_LowPower_LAB3_default_default_XC32_compile)
    list(APPEND PL10_LowPower_LAB3_default_library_list "$<TARGET_OBJECTS:PL10_LowPower_LAB3_default_default_XC32_compile>")

endif()

# Handle files with suffix cpp, for group default-XC32
if(PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_compile_cpp)
add_library(PL10_LowPower_LAB3_default_default_XC32_compile_cpp OBJECT ${PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_compile_cpp})
    PL10_LowPower_LAB3_default_default_XC32_compile_cpp_rule(PL10_LowPower_LAB3_default_default_XC32_compile_cpp)
    list(APPEND PL10_LowPower_LAB3_default_library_list "$<TARGET_OBJECTS:PL10_LowPower_LAB3_default_default_XC32_compile_cpp>")

endif()

# Handle files with suffix [cC], for group default-XC32
if(PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_dependentObject)
add_library(PL10_LowPower_LAB3_default_default_XC32_dependentObject OBJECT ${PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_dependentObject})
    PL10_LowPower_LAB3_default_default_XC32_dependentObject_rule(PL10_LowPower_LAB3_default_default_XC32_dependentObject)
    list(APPEND PL10_LowPower_LAB3_default_library_list "$<TARGET_OBJECTS:PL10_LowPower_LAB3_default_default_XC32_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC32
if(PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_bin2hex)
add_library(PL10_LowPower_LAB3_default_default_XC32_bin2hex OBJECT ${PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_bin2hex})
    PL10_LowPower_LAB3_default_default_XC32_bin2hex_rule(PL10_LowPower_LAB3_default_default_XC32_bin2hex)
    list(APPEND PL10_LowPower_LAB3_default_library_list "$<TARGET_OBJECTS:PL10_LowPower_LAB3_default_default_XC32_bin2hex>")

endif()


# Main target for this project
add_executable(PL10_LowPower_LAB3_default_image__d3Xmxts ${PL10_LowPower_LAB3_default_library_list})

set_target_properties(PL10_LowPower_LAB3_default_image__d3Xmxts PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${PL10_LowPower_LAB3_default_output_dir}")
target_link_libraries(PL10_LowPower_LAB3_default_image__d3Xmxts PRIVATE ${PL10_LowPower_LAB3_default_default_XC32_FILE_TYPE_link})

# Add the link options from the rule file.
PL10_LowPower_LAB3_default_link_rule( PL10_LowPower_LAB3_default_image__d3Xmxts)

# Call bin2hex function from the rule file
PL10_LowPower_LAB3_default_bin2hex_rule(PL10_LowPower_LAB3_default_image__d3Xmxts)

