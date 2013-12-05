#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/crt0.o \
	${OBJECTDIR}/src/ctx_sw.o \
	${OBJECTDIR}/src/debugger.o \
	${OBJECTDIR}/src/div64.o \
	${OBJECTDIR}/src/doprnt.o \
	${OBJECTDIR}/src/empty.o \
	${OBJECTDIR}/src/handlers.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/panic.o \
	${OBJECTDIR}/src/printf.o \
	${OBJECTDIR}/src/processor_structs.o \
	${OBJECTDIR}/src/processus.o \
	${OBJECTDIR}/src/screen.o \
	${OBJECTDIR}/src/sprintf.o \
	${OBJECTDIR}/src/start.o \
	${OBJECTDIR}/src/string.o \
	${OBJECTDIR}/src/strtol.o \
	${OBJECTDIR}/src/strtoul.o \
	${OBJECTDIR}/src/traitants.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/system

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/system: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/system ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/crt0.o: src/crt0.S 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/crt0.o src/crt0.S

${OBJECTDIR}/src/ctx_sw.o: src/ctx_sw.S 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/ctx_sw.o src/ctx_sw.S

${OBJECTDIR}/src/debugger.o: src/debugger.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/debugger.o src/debugger.c

${OBJECTDIR}/src/div64.o: src/div64.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/div64.o src/div64.c

${OBJECTDIR}/src/doprnt.o: src/doprnt.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/doprnt.o src/doprnt.c

${OBJECTDIR}/src/empty.o: src/empty.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/empty.o src/empty.c

${OBJECTDIR}/src/handlers.o: src/handlers.S 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/handlers.o src/handlers.S

${OBJECTDIR}/src/main.o: src/main.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/main.o src/main.c

${OBJECTDIR}/src/panic.o: src/panic.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/panic.o src/panic.c

${OBJECTDIR}/src/printf.o: src/printf.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/printf.o src/printf.c

${OBJECTDIR}/src/processor_structs.o: src/processor_structs.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/processor_structs.o src/processor_structs.c

${OBJECTDIR}/src/processus.o: src/processus.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/processus.o src/processus.c

${OBJECTDIR}/src/screen.o: src/screen.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/screen.o src/screen.c

${OBJECTDIR}/src/sprintf.o: src/sprintf.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sprintf.o src/sprintf.c

${OBJECTDIR}/src/start.o: src/start.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/start.o src/start.c

${OBJECTDIR}/src/string.o: src/string.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/string.o src/string.c

${OBJECTDIR}/src/strtol.o: src/strtol.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/strtol.o src/strtol.c

${OBJECTDIR}/src/strtoul.o: src/strtoul.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/strtoul.o src/strtoul.c

${OBJECTDIR}/src/traitants.o: src/traitants.S 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/traitants.o src/traitants.S

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/system

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
