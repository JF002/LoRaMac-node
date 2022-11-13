##
## (C)2022 Jean-François Milants
##
## License:  Revised BSD License, see LICENSE.TXT file included in the project
## Authors:  Jean-François Milants
##
##
## BL602 target specific CMake file
##

if(NOT DEFINED LINKER_SCRIPT)
message(FATAL_ERROR "No linker script defined")
endif(NOT DEFINED LINKER_SCRIPT)
message("Linker script: ${LINKER_SCRIPT}")


#---------------------------------------------------------------------------------------
# Set compiler/linker flags
#---------------------------------------------------------------------------------------

# Object build options


set(OBJECT_GEN_FLAGS "-O2 -g3 -fno-builtin -fno-jump-tables -fshort-enums -fno-common -fms-extensions -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -ffast-math -Wall -Wshift-negative-value -Wchar-subscripts -Wformat -Wuninitialized -Winit-self -Wignored-qualifiers -Wunused -Wundef -msmall-data-limit=4 -Wtype-limits -march=rv32imafc -mabi=ilp32f")

set(CMAKE_C_FLAGS "${OBJECT_GEN_FLAGS} -std=gnu99 " CACHE INTERNAL "C Compiler options")
set(CMAKE_CXX_FLAGS "${OBJECT_GEN_FLAGS} -std=c++11 " CACHE INTERNAL "C++ Compiler options")
set(CMAKE_ASM_FLAGS "${OBJECT_GEN_FLAGS} -x assembler-with-cpp " CACHE INTERNAL "ASM Compiler options")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--cref -Wl,--gc-sections -nostartfiles -g3 -fms-extensions -ffunction-sections -fdata-sections -Wall -Wchar-subscripts -std=c99 --specs=nano.specs -march=rv32imafc -mabi=ilp32f -Wl,-m,elf32lriscv -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_PROJECT_NAME}.map" CACHE INTERNAL "Linker options")
