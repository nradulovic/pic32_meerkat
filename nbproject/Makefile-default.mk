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
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=application/source/bsp.c application/source/main.c application/source/support.c driver/source/clock.c neon/eds/port/pic32-none-gcc/p_core.c neon/eds/source/epa.c neon/eds/source/equeue.c neon/eds/source/etimer.c neon/eds/source/event.c neon/eds/source/heap.c neon/eds/source/mem.c neon/eds/source/pool.c neon/eds/source/sched.c neon/eds/source/smp.c neon/eds/source/static.c neon/eds/source/timer.c neon/lib/source/bits.c neon/lib/source/checksum.c neon/lib/source/string/num_conv.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/application/source/bsp.o ${OBJECTDIR}/application/source/main.o ${OBJECTDIR}/application/source/support.o ${OBJECTDIR}/driver/source/clock.o ${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o ${OBJECTDIR}/neon/eds/source/epa.o ${OBJECTDIR}/neon/eds/source/equeue.o ${OBJECTDIR}/neon/eds/source/etimer.o ${OBJECTDIR}/neon/eds/source/event.o ${OBJECTDIR}/neon/eds/source/heap.o ${OBJECTDIR}/neon/eds/source/mem.o ${OBJECTDIR}/neon/eds/source/pool.o ${OBJECTDIR}/neon/eds/source/sched.o ${OBJECTDIR}/neon/eds/source/smp.o ${OBJECTDIR}/neon/eds/source/static.o ${OBJECTDIR}/neon/eds/source/timer.o ${OBJECTDIR}/neon/lib/source/bits.o ${OBJECTDIR}/neon/lib/source/checksum.o ${OBJECTDIR}/neon/lib/source/string/num_conv.o
POSSIBLE_DEPFILES=${OBJECTDIR}/application/source/bsp.o.d ${OBJECTDIR}/application/source/main.o.d ${OBJECTDIR}/application/source/support.o.d ${OBJECTDIR}/driver/source/clock.o.d ${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o.d ${OBJECTDIR}/neon/eds/source/epa.o.d ${OBJECTDIR}/neon/eds/source/equeue.o.d ${OBJECTDIR}/neon/eds/source/etimer.o.d ${OBJECTDIR}/neon/eds/source/event.o.d ${OBJECTDIR}/neon/eds/source/heap.o.d ${OBJECTDIR}/neon/eds/source/mem.o.d ${OBJECTDIR}/neon/eds/source/pool.o.d ${OBJECTDIR}/neon/eds/source/sched.o.d ${OBJECTDIR}/neon/eds/source/smp.o.d ${OBJECTDIR}/neon/eds/source/static.o.d ${OBJECTDIR}/neon/eds/source/timer.o.d ${OBJECTDIR}/neon/lib/source/bits.o.d ${OBJECTDIR}/neon/lib/source/checksum.o.d ${OBJECTDIR}/neon/lib/source/string/num_conv.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/application/source/bsp.o ${OBJECTDIR}/application/source/main.o ${OBJECTDIR}/application/source/support.o ${OBJECTDIR}/driver/source/clock.o ${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o ${OBJECTDIR}/neon/eds/source/epa.o ${OBJECTDIR}/neon/eds/source/equeue.o ${OBJECTDIR}/neon/eds/source/etimer.o ${OBJECTDIR}/neon/eds/source/event.o ${OBJECTDIR}/neon/eds/source/heap.o ${OBJECTDIR}/neon/eds/source/mem.o ${OBJECTDIR}/neon/eds/source/pool.o ${OBJECTDIR}/neon/eds/source/sched.o ${OBJECTDIR}/neon/eds/source/smp.o ${OBJECTDIR}/neon/eds/source/static.o ${OBJECTDIR}/neon/eds/source/timer.o ${OBJECTDIR}/neon/lib/source/bits.o ${OBJECTDIR}/neon/lib/source/checksum.o ${OBJECTDIR}/neon/lib/source/string/num_conv.o

# Source Files
SOURCEFILES=application/source/bsp.c application/source/main.c application/source/support.c driver/source/clock.c neon/eds/port/pic32-none-gcc/p_core.c neon/eds/source/epa.c neon/eds/source/equeue.c neon/eds/source/etimer.c neon/eds/source/event.c neon/eds/source/heap.c neon/eds/source/mem.c neon/eds/source/pool.c neon/eds/source/sched.c neon/eds/source/smp.c neon/eds/source/static.c neon/eds/source/timer.c neon/lib/source/bits.c neon/lib/source/checksum.c neon/lib/source/string/num_conv.c


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
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/application/source/bsp.o: application/source/bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/application/source" 
	@${RM} ${OBJECTDIR}/application/source/bsp.o.d 
	@${RM} ${OBJECTDIR}/application/source/bsp.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/application/source/bsp.o.d" -o ${OBJECTDIR}/application/source/bsp.o application/source/bsp.c      -Wextra
	
${OBJECTDIR}/application/source/main.o: application/source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/application/source" 
	@${RM} ${OBJECTDIR}/application/source/main.o.d 
	@${RM} ${OBJECTDIR}/application/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/application/source/main.o.d" -o ${OBJECTDIR}/application/source/main.o application/source/main.c      -Wextra
	
${OBJECTDIR}/application/source/support.o: application/source/support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/application/source" 
	@${RM} ${OBJECTDIR}/application/source/support.o.d 
	@${RM} ${OBJECTDIR}/application/source/support.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/support.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/application/source/support.o.d" -o ${OBJECTDIR}/application/source/support.o application/source/support.c      -Wextra
	
${OBJECTDIR}/driver/source/clock.o: driver/source/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/driver/source" 
	@${RM} ${OBJECTDIR}/driver/source/clock.o.d 
	@${RM} ${OBJECTDIR}/driver/source/clock.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/driver/source/clock.o.d" -o ${OBJECTDIR}/driver/source/clock.o driver/source/clock.c      -Wextra
	
${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o: neon/eds/port/pic32-none-gcc/p_core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/port/pic32-none-gcc" 
	@${RM} ${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o.d" -o ${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o neon/eds/port/pic32-none-gcc/p_core.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/epa.o: neon/eds/source/epa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/epa.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/epa.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/epa.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/epa.o.d" -o ${OBJECTDIR}/neon/eds/source/epa.o neon/eds/source/epa.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/equeue.o: neon/eds/source/equeue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/equeue.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/equeue.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/equeue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/equeue.o.d" -o ${OBJECTDIR}/neon/eds/source/equeue.o neon/eds/source/equeue.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/etimer.o: neon/eds/source/etimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/etimer.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/etimer.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/etimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/etimer.o.d" -o ${OBJECTDIR}/neon/eds/source/etimer.o neon/eds/source/etimer.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/event.o: neon/eds/source/event.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/event.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/event.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/event.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/event.o.d" -o ${OBJECTDIR}/neon/eds/source/event.o neon/eds/source/event.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/heap.o: neon/eds/source/heap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/heap.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/heap.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/heap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/heap.o.d" -o ${OBJECTDIR}/neon/eds/source/heap.o neon/eds/source/heap.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/mem.o: neon/eds/source/mem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/mem.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/mem.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/mem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/mem.o.d" -o ${OBJECTDIR}/neon/eds/source/mem.o neon/eds/source/mem.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/pool.o: neon/eds/source/pool.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/pool.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/pool.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/pool.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/pool.o.d" -o ${OBJECTDIR}/neon/eds/source/pool.o neon/eds/source/pool.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/sched.o: neon/eds/source/sched.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/sched.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/sched.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/sched.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/sched.o.d" -o ${OBJECTDIR}/neon/eds/source/sched.o neon/eds/source/sched.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/smp.o: neon/eds/source/smp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/smp.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/smp.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/smp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/smp.o.d" -o ${OBJECTDIR}/neon/eds/source/smp.o neon/eds/source/smp.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/static.o: neon/eds/source/static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/static.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/static.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/static.o.d" -o ${OBJECTDIR}/neon/eds/source/static.o neon/eds/source/static.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/timer.o: neon/eds/source/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/timer.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/timer.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/timer.o.d" -o ${OBJECTDIR}/neon/eds/source/timer.o neon/eds/source/timer.c      -Wextra
	
${OBJECTDIR}/neon/lib/source/bits.o: neon/lib/source/bits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/lib/source" 
	@${RM} ${OBJECTDIR}/neon/lib/source/bits.o.d 
	@${RM} ${OBJECTDIR}/neon/lib/source/bits.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/lib/source/bits.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/lib/source/bits.o.d" -o ${OBJECTDIR}/neon/lib/source/bits.o neon/lib/source/bits.c      -Wextra
	
${OBJECTDIR}/neon/lib/source/checksum.o: neon/lib/source/checksum.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/lib/source" 
	@${RM} ${OBJECTDIR}/neon/lib/source/checksum.o.d 
	@${RM} ${OBJECTDIR}/neon/lib/source/checksum.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/lib/source/checksum.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/lib/source/checksum.o.d" -o ${OBJECTDIR}/neon/lib/source/checksum.o neon/lib/source/checksum.c      -Wextra
	
${OBJECTDIR}/neon/lib/source/string/num_conv.o: neon/lib/source/string/num_conv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/lib/source/string" 
	@${RM} ${OBJECTDIR}/neon/lib/source/string/num_conv.o.d 
	@${RM} ${OBJECTDIR}/neon/lib/source/string/num_conv.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/lib/source/string/num_conv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/lib/source/string/num_conv.o.d" -o ${OBJECTDIR}/neon/lib/source/string/num_conv.o neon/lib/source/string/num_conv.c      -Wextra
	
else
${OBJECTDIR}/application/source/bsp.o: application/source/bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/application/source" 
	@${RM} ${OBJECTDIR}/application/source/bsp.o.d 
	@${RM} ${OBJECTDIR}/application/source/bsp.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/application/source/bsp.o.d" -o ${OBJECTDIR}/application/source/bsp.o application/source/bsp.c      -Wextra
	
${OBJECTDIR}/application/source/main.o: application/source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/application/source" 
	@${RM} ${OBJECTDIR}/application/source/main.o.d 
	@${RM} ${OBJECTDIR}/application/source/main.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/application/source/main.o.d" -o ${OBJECTDIR}/application/source/main.o application/source/main.c      -Wextra
	
${OBJECTDIR}/application/source/support.o: application/source/support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/application/source" 
	@${RM} ${OBJECTDIR}/application/source/support.o.d 
	@${RM} ${OBJECTDIR}/application/source/support.o 
	@${FIXDEPS} "${OBJECTDIR}/application/source/support.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/application/source/support.o.d" -o ${OBJECTDIR}/application/source/support.o application/source/support.c      -Wextra
	
${OBJECTDIR}/driver/source/clock.o: driver/source/clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/driver/source" 
	@${RM} ${OBJECTDIR}/driver/source/clock.o.d 
	@${RM} ${OBJECTDIR}/driver/source/clock.o 
	@${FIXDEPS} "${OBJECTDIR}/driver/source/clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/driver/source/clock.o.d" -o ${OBJECTDIR}/driver/source/clock.o driver/source/clock.c      -Wextra
	
${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o: neon/eds/port/pic32-none-gcc/p_core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/port/pic32-none-gcc" 
	@${RM} ${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o.d" -o ${OBJECTDIR}/neon/eds/port/pic32-none-gcc/p_core.o neon/eds/port/pic32-none-gcc/p_core.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/epa.o: neon/eds/source/epa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/epa.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/epa.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/epa.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/epa.o.d" -o ${OBJECTDIR}/neon/eds/source/epa.o neon/eds/source/epa.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/equeue.o: neon/eds/source/equeue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/equeue.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/equeue.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/equeue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/equeue.o.d" -o ${OBJECTDIR}/neon/eds/source/equeue.o neon/eds/source/equeue.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/etimer.o: neon/eds/source/etimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/etimer.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/etimer.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/etimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/etimer.o.d" -o ${OBJECTDIR}/neon/eds/source/etimer.o neon/eds/source/etimer.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/event.o: neon/eds/source/event.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/event.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/event.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/event.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/event.o.d" -o ${OBJECTDIR}/neon/eds/source/event.o neon/eds/source/event.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/heap.o: neon/eds/source/heap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/heap.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/heap.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/heap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/heap.o.d" -o ${OBJECTDIR}/neon/eds/source/heap.o neon/eds/source/heap.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/mem.o: neon/eds/source/mem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/mem.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/mem.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/mem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/mem.o.d" -o ${OBJECTDIR}/neon/eds/source/mem.o neon/eds/source/mem.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/pool.o: neon/eds/source/pool.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/pool.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/pool.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/pool.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/pool.o.d" -o ${OBJECTDIR}/neon/eds/source/pool.o neon/eds/source/pool.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/sched.o: neon/eds/source/sched.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/sched.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/sched.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/sched.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/sched.o.d" -o ${OBJECTDIR}/neon/eds/source/sched.o neon/eds/source/sched.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/smp.o: neon/eds/source/smp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/smp.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/smp.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/smp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/smp.o.d" -o ${OBJECTDIR}/neon/eds/source/smp.o neon/eds/source/smp.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/static.o: neon/eds/source/static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/static.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/static.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/static.o.d" -o ${OBJECTDIR}/neon/eds/source/static.o neon/eds/source/static.c      -Wextra
	
${OBJECTDIR}/neon/eds/source/timer.o: neon/eds/source/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/eds/source" 
	@${RM} ${OBJECTDIR}/neon/eds/source/timer.o.d 
	@${RM} ${OBJECTDIR}/neon/eds/source/timer.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/eds/source/timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/eds/source/timer.o.d" -o ${OBJECTDIR}/neon/eds/source/timer.o neon/eds/source/timer.c      -Wextra
	
${OBJECTDIR}/neon/lib/source/bits.o: neon/lib/source/bits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/lib/source" 
	@${RM} ${OBJECTDIR}/neon/lib/source/bits.o.d 
	@${RM} ${OBJECTDIR}/neon/lib/source/bits.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/lib/source/bits.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/lib/source/bits.o.d" -o ${OBJECTDIR}/neon/lib/source/bits.o neon/lib/source/bits.c      -Wextra
	
${OBJECTDIR}/neon/lib/source/checksum.o: neon/lib/source/checksum.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/lib/source" 
	@${RM} ${OBJECTDIR}/neon/lib/source/checksum.o.d 
	@${RM} ${OBJECTDIR}/neon/lib/source/checksum.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/lib/source/checksum.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/lib/source/checksum.o.d" -o ${OBJECTDIR}/neon/lib/source/checksum.o neon/lib/source/checksum.c      -Wextra
	
${OBJECTDIR}/neon/lib/source/string/num_conv.o: neon/lib/source/string/num_conv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/neon/lib/source/string" 
	@${RM} ${OBJECTDIR}/neon/lib/source/string/num_conv.o.d 
	@${RM} ${OBJECTDIR}/neon/lib/source/string/num_conv.o 
	@${FIXDEPS} "${OBJECTDIR}/neon/lib/source/string/num_conv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -mips16 -mno-float -D_PLIB_DISABLE_LEGACY -I"driver/include" -I"application/include" -I"neon/eds/include" -I"neon/eds/port/pic32-none-gcc" -I"neon/lib/include" -I"neon/drivers/include" -Wall -pedantic -MMD -MF "${OBJECTDIR}/neon/lib/source/string/num_conv.o.d" -o ${OBJECTDIR}/neon/lib/source/string/num_conv.o neon/lib/source/string/num_conv.c      -Wextra
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION) -Os -mno-float -o dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}              -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,--defsym=_min_stack_size=4096,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--warn-section-align
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -Os -mno-float -o dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}            -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_stack_size=4096,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--warn-section-align
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/meerkat.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
