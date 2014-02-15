#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=source/driver/lld_spi1.c source/driver/spi.c source/driver/uart.c source/driver/lld_spis.c esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c esolid-base/port/pic32-none-gcc/mips-m4k/intr.c esolid-base/src/debug.c esolid-base/src/base.c esolid-base/src/error.c esolid-base/src/prio_queue.c esolid-eds/src/smp.c esolid-eds/src/event.c esolid-eds/src/epa.c esolid-mem/src/mem_class.c esolid-mem/src/heap.c esolid-mem/src/static.c esolid-mem/src/pool.c esolid-vtimer/src/vtimer.c source/test/test_spi.c source/main.c source/bsp.c source/support.c source/driver/codec.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/source/driver/lld_spi1.o ${OBJECTDIR}/source/driver/spi.o ${OBJECTDIR}/source/driver/uart.o ${OBJECTDIR}/source/driver/lld_spis.o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o ${OBJECTDIR}/esolid-base/src/debug.o ${OBJECTDIR}/esolid-base/src/base.o ${OBJECTDIR}/esolid-base/src/error.o ${OBJECTDIR}/esolid-base/src/prio_queue.o ${OBJECTDIR}/esolid-eds/src/smp.o ${OBJECTDIR}/esolid-eds/src/event.o ${OBJECTDIR}/esolid-eds/src/epa.o ${OBJECTDIR}/esolid-mem/src/mem_class.o ${OBJECTDIR}/esolid-mem/src/heap.o ${OBJECTDIR}/esolid-mem/src/static.o ${OBJECTDIR}/esolid-mem/src/pool.o ${OBJECTDIR}/esolid-vtimer/src/vtimer.o ${OBJECTDIR}/source/test/test_spi.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/bsp.o ${OBJECTDIR}/source/support.o ${OBJECTDIR}/source/driver/codec.o
POSSIBLE_DEPFILES=${OBJECTDIR}/source/driver/lld_spi1.o.d ${OBJECTDIR}/source/driver/spi.o.d ${OBJECTDIR}/source/driver/uart.o.d ${OBJECTDIR}/source/driver/lld_spis.o.d ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d ${OBJECTDIR}/esolid-base/src/debug.o.d ${OBJECTDIR}/esolid-base/src/base.o.d ${OBJECTDIR}/esolid-base/src/error.o.d ${OBJECTDIR}/esolid-base/src/prio_queue.o.d ${OBJECTDIR}/esolid-eds/src/smp.o.d ${OBJECTDIR}/esolid-eds/src/event.o.d ${OBJECTDIR}/esolid-eds/src/epa.o.d ${OBJECTDIR}/esolid-mem/src/mem_class.o.d ${OBJECTDIR}/esolid-mem/src/heap.o.d ${OBJECTDIR}/esolid-mem/src/static.o.d ${OBJECTDIR}/esolid-mem/src/pool.o.d ${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d ${OBJECTDIR}/source/test/test_spi.o.d ${OBJECTDIR}/source/main.o.d ${OBJECTDIR}/source/bsp.o.d ${OBJECTDIR}/source/support.o.d ${OBJECTDIR}/source/driver/codec.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/source/driver/lld_spi1.o ${OBJECTDIR}/source/driver/spi.o ${OBJECTDIR}/source/driver/uart.o ${OBJECTDIR}/source/driver/lld_spis.o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o ${OBJECTDIR}/esolid-base/src/debug.o ${OBJECTDIR}/esolid-base/src/base.o ${OBJECTDIR}/esolid-base/src/error.o ${OBJECTDIR}/esolid-base/src/prio_queue.o ${OBJECTDIR}/esolid-eds/src/smp.o ${OBJECTDIR}/esolid-eds/src/event.o ${OBJECTDIR}/esolid-eds/src/epa.o ${OBJECTDIR}/esolid-mem/src/mem_class.o ${OBJECTDIR}/esolid-mem/src/heap.o ${OBJECTDIR}/esolid-mem/src/static.o ${OBJECTDIR}/esolid-mem/src/pool.o ${OBJECTDIR}/esolid-vtimer/src/vtimer.o ${OBJECTDIR}/source/test/test_spi.o ${OBJECTDIR}/source/main.o ${OBJECTDIR}/source/bsp.o ${OBJECTDIR}/source/support.o ${OBJECTDIR}/source/driver/codec.o

# Source Files
SOURCEFILES=source/driver/lld_spi1.c source/driver/spi.c source/driver/uart.c source/driver/lld_spis.c esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c esolid-base/port/pic32-none-gcc/mips-m4k/intr.c esolid-base/src/debug.c esolid-base/src/base.c esolid-base/src/error.c esolid-base/src/prio_queue.c esolid-eds/src/smp.c esolid-eds/src/event.c esolid-eds/src/epa.c esolid-mem/src/mem_class.c esolid-mem/src/heap.c esolid-mem/src/static.c esolid-mem/src/pool.c esolid-vtimer/src/vtimer.c source/test/test_spi.c source/main.c source/bsp.c source/support.c source/driver/codec.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX150F128D
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/source/driver/lld_spi1.o: source/driver/lld_spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/driver 
	@${RM} ${OBJECTDIR}/source/driver/lld_spi1.o.d 
	@${RM} ${OBJECTDIR}/source/driver/lld_spi1.o 
	@${FIXDEPS} "${OBJECTDIR}/source/driver/lld_spi1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/driver/lld_spi1.o.d" -o ${OBJECTDIR}/source/driver/lld_spi1.o source/driver/lld_spi1.c   -save-temps=obj
	
${OBJECTDIR}/source/driver/spi.o: source/driver/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/driver 
	@${RM} ${OBJECTDIR}/source/driver/spi.o.d 
	@${RM} ${OBJECTDIR}/source/driver/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/source/driver/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/driver/spi.o.d" -o ${OBJECTDIR}/source/driver/spi.o source/driver/spi.c   -save-temps=obj
	
${OBJECTDIR}/source/driver/uart.o: source/driver/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/driver 
	@${RM} ${OBJECTDIR}/source/driver/uart.o.d 
	@${RM} ${OBJECTDIR}/source/driver/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/source/driver/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/driver/uart.o.d" -o ${OBJECTDIR}/source/driver/uart.o source/driver/uart.c   -save-temps=obj
	
${OBJECTDIR}/source/driver/lld_spis.o: source/driver/lld_spis.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/driver 
	@${RM} ${OBJECTDIR}/source/driver/lld_spis.o.d 
	@${RM} ${OBJECTDIR}/source/driver/lld_spis.o 
	@${FIXDEPS} "${OBJECTDIR}/source/driver/lld_spis.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/driver/lld_spis.o.d" -o ${OBJECTDIR}/source/driver/lld_spis.o source/driver/lld_spis.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o: esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d" -o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o: esolid-base/port/pic32-none-gcc/mips-m4k/intr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d" -o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o esolid-base/port/pic32-none-gcc/mips-m4k/intr.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/src/debug.o: esolid-base/src/debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/debug.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/debug.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/src/debug.o.d" -o ${OBJECTDIR}/esolid-base/src/debug.o esolid-base/src/debug.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/src/base.o: esolid-base/src/base.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/base.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/base.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/base.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/src/base.o.d" -o ${OBJECTDIR}/esolid-base/src/base.o esolid-base/src/base.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/src/error.o: esolid-base/src/error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/error.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/error.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/error.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/src/error.o.d" -o ${OBJECTDIR}/esolid-base/src/error.o esolid-base/src/error.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/src/prio_queue.o: esolid-base/src/prio_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/prio_queue.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/prio_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/prio_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/src/prio_queue.o.d" -o ${OBJECTDIR}/esolid-base/src/prio_queue.o esolid-base/src/prio_queue.c   -save-temps=obj
	
${OBJECTDIR}/esolid-eds/src/smp.o: esolid-eds/src/smp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/smp.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/smp.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/smp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-eds/src/smp.o.d" -o ${OBJECTDIR}/esolid-eds/src/smp.o esolid-eds/src/smp.c   -save-temps=obj
	
${OBJECTDIR}/esolid-eds/src/event.o: esolid-eds/src/event.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/event.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/event.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/event.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-eds/src/event.o.d" -o ${OBJECTDIR}/esolid-eds/src/event.o esolid-eds/src/event.c   -save-temps=obj
	
${OBJECTDIR}/esolid-eds/src/epa.o: esolid-eds/src/epa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/epa.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/epa.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/epa.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-eds/src/epa.o.d" -o ${OBJECTDIR}/esolid-eds/src/epa.o esolid-eds/src/epa.c   -save-temps=obj
	
${OBJECTDIR}/esolid-mem/src/mem_class.o: esolid-mem/src/mem_class.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/mem_class.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/mem_class.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/mem_class.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-mem/src/mem_class.o.d" -o ${OBJECTDIR}/esolid-mem/src/mem_class.o esolid-mem/src/mem_class.c   -save-temps=obj
	
${OBJECTDIR}/esolid-mem/src/heap.o: esolid-mem/src/heap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/heap.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/heap.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/heap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-mem/src/heap.o.d" -o ${OBJECTDIR}/esolid-mem/src/heap.o esolid-mem/src/heap.c   -save-temps=obj
	
${OBJECTDIR}/esolid-mem/src/static.o: esolid-mem/src/static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/static.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/static.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-mem/src/static.o.d" -o ${OBJECTDIR}/esolid-mem/src/static.o esolid-mem/src/static.c   -save-temps=obj
	
${OBJECTDIR}/esolid-mem/src/pool.o: esolid-mem/src/pool.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/pool.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/pool.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/pool.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-mem/src/pool.o.d" -o ${OBJECTDIR}/esolid-mem/src/pool.o esolid-mem/src/pool.c   -save-temps=obj
	
${OBJECTDIR}/esolid-vtimer/src/vtimer.o: esolid-vtimer/src/vtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-vtimer/src 
	@${RM} ${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d 
	@${RM} ${OBJECTDIR}/esolid-vtimer/src/vtimer.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d" -o ${OBJECTDIR}/esolid-vtimer/src/vtimer.o esolid-vtimer/src/vtimer.c   -save-temps=obj
	
${OBJECTDIR}/source/test/test_spi.o: source/test/test_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/test 
	@${RM} ${OBJECTDIR}/source/test/test_spi.o.d 
	@${RM} ${OBJECTDIR}/source/test/test_spi.o 
	@${FIXDEPS} "${OBJECTDIR}/source/test/test_spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/test/test_spi.o.d" -o ${OBJECTDIR}/source/test/test_spi.o source/test/test_spi.c   -save-temps=obj
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/main.o.d" -o ${OBJECTDIR}/source/main.o source/main.c   -save-temps=obj
	
${OBJECTDIR}/source/bsp.o: source/bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/bsp.o.d 
	@${RM} ${OBJECTDIR}/source/bsp.o 
	@${FIXDEPS} "${OBJECTDIR}/source/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/bsp.o.d" -o ${OBJECTDIR}/source/bsp.o source/bsp.c   -save-temps=obj
	
${OBJECTDIR}/source/support.o: source/support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/support.o.d 
	@${RM} ${OBJECTDIR}/source/support.o 
	@${FIXDEPS} "${OBJECTDIR}/source/support.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/support.o.d" -o ${OBJECTDIR}/source/support.o source/support.c   -save-temps=obj
	
${OBJECTDIR}/source/driver/codec.o: source/driver/codec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/driver 
	@${RM} ${OBJECTDIR}/source/driver/codec.o.d 
	@${RM} ${OBJECTDIR}/source/driver/codec.o 
	@${FIXDEPS} "${OBJECTDIR}/source/driver/codec.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/driver/codec.o.d" -o ${OBJECTDIR}/source/driver/codec.o source/driver/codec.c   -save-temps=obj
	
else
${OBJECTDIR}/source/driver/lld_spi1.o: source/driver/lld_spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/driver 
	@${RM} ${OBJECTDIR}/source/driver/lld_spi1.o.d 
	@${RM} ${OBJECTDIR}/source/driver/lld_spi1.o 
	@${FIXDEPS} "${OBJECTDIR}/source/driver/lld_spi1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/driver/lld_spi1.o.d" -o ${OBJECTDIR}/source/driver/lld_spi1.o source/driver/lld_spi1.c   -save-temps=obj
	
${OBJECTDIR}/source/driver/spi.o: source/driver/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/driver 
	@${RM} ${OBJECTDIR}/source/driver/spi.o.d 
	@${RM} ${OBJECTDIR}/source/driver/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/source/driver/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/driver/spi.o.d" -o ${OBJECTDIR}/source/driver/spi.o source/driver/spi.c   -save-temps=obj
	
${OBJECTDIR}/source/driver/uart.o: source/driver/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/driver 
	@${RM} ${OBJECTDIR}/source/driver/uart.o.d 
	@${RM} ${OBJECTDIR}/source/driver/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/source/driver/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/driver/uart.o.d" -o ${OBJECTDIR}/source/driver/uart.o source/driver/uart.c   -save-temps=obj
	
${OBJECTDIR}/source/driver/lld_spis.o: source/driver/lld_spis.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/driver 
	@${RM} ${OBJECTDIR}/source/driver/lld_spis.o.d 
	@${RM} ${OBJECTDIR}/source/driver/lld_spis.o 
	@${FIXDEPS} "${OBJECTDIR}/source/driver/lld_spis.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/driver/lld_spis.o.d" -o ${OBJECTDIR}/source/driver/lld_spis.o source/driver/lld_spis.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o: esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o.d" -o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/cpu.o esolid-base/port/pic32-none-gcc/mips-m4k/cpu.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o: esolid-base/port/pic32-none-gcc/mips-m4k/intr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o.d" -o ${OBJECTDIR}/esolid-base/port/pic32-none-gcc/mips-m4k/intr.o esolid-base/port/pic32-none-gcc/mips-m4k/intr.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/src/debug.o: esolid-base/src/debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/debug.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/debug.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/src/debug.o.d" -o ${OBJECTDIR}/esolid-base/src/debug.o esolid-base/src/debug.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/src/base.o: esolid-base/src/base.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/base.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/base.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/base.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/src/base.o.d" -o ${OBJECTDIR}/esolid-base/src/base.o esolid-base/src/base.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/src/error.o: esolid-base/src/error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/error.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/error.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/error.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/src/error.o.d" -o ${OBJECTDIR}/esolid-base/src/error.o esolid-base/src/error.c   -save-temps=obj
	
${OBJECTDIR}/esolid-base/src/prio_queue.o: esolid-base/src/prio_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-base/src 
	@${RM} ${OBJECTDIR}/esolid-base/src/prio_queue.o.d 
	@${RM} ${OBJECTDIR}/esolid-base/src/prio_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-base/src/prio_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-base/src/prio_queue.o.d" -o ${OBJECTDIR}/esolid-base/src/prio_queue.o esolid-base/src/prio_queue.c   -save-temps=obj
	
${OBJECTDIR}/esolid-eds/src/smp.o: esolid-eds/src/smp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/smp.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/smp.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/smp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-eds/src/smp.o.d" -o ${OBJECTDIR}/esolid-eds/src/smp.o esolid-eds/src/smp.c   -save-temps=obj
	
${OBJECTDIR}/esolid-eds/src/event.o: esolid-eds/src/event.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/event.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/event.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/event.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-eds/src/event.o.d" -o ${OBJECTDIR}/esolid-eds/src/event.o esolid-eds/src/event.c   -save-temps=obj
	
${OBJECTDIR}/esolid-eds/src/epa.o: esolid-eds/src/epa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-eds/src 
	@${RM} ${OBJECTDIR}/esolid-eds/src/epa.o.d 
	@${RM} ${OBJECTDIR}/esolid-eds/src/epa.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-eds/src/epa.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-eds/src/epa.o.d" -o ${OBJECTDIR}/esolid-eds/src/epa.o esolid-eds/src/epa.c   -save-temps=obj
	
${OBJECTDIR}/esolid-mem/src/mem_class.o: esolid-mem/src/mem_class.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/mem_class.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/mem_class.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/mem_class.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-mem/src/mem_class.o.d" -o ${OBJECTDIR}/esolid-mem/src/mem_class.o esolid-mem/src/mem_class.c   -save-temps=obj
	
${OBJECTDIR}/esolid-mem/src/heap.o: esolid-mem/src/heap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/heap.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/heap.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/heap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-mem/src/heap.o.d" -o ${OBJECTDIR}/esolid-mem/src/heap.o esolid-mem/src/heap.c   -save-temps=obj
	
${OBJECTDIR}/esolid-mem/src/static.o: esolid-mem/src/static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/static.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/static.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-mem/src/static.o.d" -o ${OBJECTDIR}/esolid-mem/src/static.o esolid-mem/src/static.c   -save-temps=obj
	
${OBJECTDIR}/esolid-mem/src/pool.o: esolid-mem/src/pool.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-mem/src 
	@${RM} ${OBJECTDIR}/esolid-mem/src/pool.o.d 
	@${RM} ${OBJECTDIR}/esolid-mem/src/pool.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-mem/src/pool.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-mem/src/pool.o.d" -o ${OBJECTDIR}/esolid-mem/src/pool.o esolid-mem/src/pool.c   -save-temps=obj
	
${OBJECTDIR}/esolid-vtimer/src/vtimer.o: esolid-vtimer/src/vtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/esolid-vtimer/src 
	@${RM} ${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d 
	@${RM} ${OBJECTDIR}/esolid-vtimer/src/vtimer.o 
	@${FIXDEPS} "${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/esolid-vtimer/src/vtimer.o.d" -o ${OBJECTDIR}/esolid-vtimer/src/vtimer.o esolid-vtimer/src/vtimer.c   -save-temps=obj
	
${OBJECTDIR}/source/test/test_spi.o: source/test/test_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/test 
	@${RM} ${OBJECTDIR}/source/test/test_spi.o.d 
	@${RM} ${OBJECTDIR}/source/test/test_spi.o 
	@${FIXDEPS} "${OBJECTDIR}/source/test/test_spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/test/test_spi.o.d" -o ${OBJECTDIR}/source/test/test_spi.o source/test/test_spi.c   -save-temps=obj
	
${OBJECTDIR}/source/main.o: source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/main.o.d 
	@${RM} ${OBJECTDIR}/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/main.o.d" -o ${OBJECTDIR}/source/main.o source/main.c   -save-temps=obj
	
${OBJECTDIR}/source/bsp.o: source/bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/bsp.o.d 
	@${RM} ${OBJECTDIR}/source/bsp.o 
	@${FIXDEPS} "${OBJECTDIR}/source/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/bsp.o.d" -o ${OBJECTDIR}/source/bsp.o source/bsp.c   -save-temps=obj
	
${OBJECTDIR}/source/support.o: source/support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/support.o.d 
	@${RM} ${OBJECTDIR}/source/support.o 
	@${FIXDEPS} "${OBJECTDIR}/source/support.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/support.o.d" -o ${OBJECTDIR}/source/support.o source/support.c   -save-temps=obj
	
${OBJECTDIR}/source/driver/codec.o: source/driver/codec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/driver 
	@${RM} ${OBJECTDIR}/source/driver/codec.o.d 
	@${RM} ${OBJECTDIR}/source/driver/codec.o 
	@${FIXDEPS} "${OBJECTDIR}/source/driver/codec.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -DINLINE_API="static inline" -DINLINE="static inline" -DUSE_CRYPTO_HW -D_PLIB_DISABLE_LEGACY -I"." -I"esolid-base/inc" -I"esolid-base/port/pic32-none-gcc/common" -I"esolid-base/port/pic32-none-gcc/mips-m4k" -I"esolid-base/port/pic32-none-gcc/pic32mx150f128d" -I"esolid-mem/inc" -I"esolid-vtimer/inc" -I"esolid-eds/inc" -I"include" -I"include/test" -Wall -pedantic -MMD -MF "${OBJECTDIR}/source/driver/codec.o.d" -o ${OBJECTDIR}/source/driver/codec.o source/driver/codec.c   -save-temps=obj
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -save-temps=obj  -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--warn-section-align
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -save-temps=obj -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--warn-section-align
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
