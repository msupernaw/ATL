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
CCC=clang++
CXX=clang++
FC=g77
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkAlgorithm.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkBitset.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkDeque.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkHash.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkHeap.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkList.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkMap.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkSet.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkSort.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkString.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkTupleVector.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkVector.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/EASTLBenchmark.o \
	${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/main.o \
	${OBJECTDIR}/lib/third_party/EASTL/source/allocator_eastl.o \
	${OBJECTDIR}/lib/third_party/EASTL/source/assert.o \
	${OBJECTDIR}/lib/third_party/EASTL/source/fixed_pool.o \
	${OBJECTDIR}/lib/third_party/EASTL/source/hashtable.o \
	${OBJECTDIR}/lib/third_party/EASTL/source/intrusive_list.o \
	${OBJECTDIR}/lib/third_party/EASTL/source/numeric_limits.o \
	${OBJECTDIR}/lib/third_party/EASTL/source/red_black_tree.o \
	${OBJECTDIR}/lib/third_party/EASTL/source/string.o \
	${OBJECTDIR}/lib/third_party/EASTL/source/thread_support.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/atl

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/atl: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/atl ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkAlgorithm.o: lib/third_party/EASTL/benchmark/source/BenchmarkAlgorithm.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkAlgorithm.o lib/third_party/EASTL/benchmark/source/BenchmarkAlgorithm.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkBitset.o: lib/third_party/EASTL/benchmark/source/BenchmarkBitset.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkBitset.o lib/third_party/EASTL/benchmark/source/BenchmarkBitset.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkDeque.o: lib/third_party/EASTL/benchmark/source/BenchmarkDeque.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkDeque.o lib/third_party/EASTL/benchmark/source/BenchmarkDeque.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkHash.o: lib/third_party/EASTL/benchmark/source/BenchmarkHash.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkHash.o lib/third_party/EASTL/benchmark/source/BenchmarkHash.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkHeap.o: lib/third_party/EASTL/benchmark/source/BenchmarkHeap.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkHeap.o lib/third_party/EASTL/benchmark/source/BenchmarkHeap.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkList.o: lib/third_party/EASTL/benchmark/source/BenchmarkList.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkList.o lib/third_party/EASTL/benchmark/source/BenchmarkList.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkMap.o: lib/third_party/EASTL/benchmark/source/BenchmarkMap.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkMap.o lib/third_party/EASTL/benchmark/source/BenchmarkMap.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkSet.o: lib/third_party/EASTL/benchmark/source/BenchmarkSet.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkSet.o lib/third_party/EASTL/benchmark/source/BenchmarkSet.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkSort.o: lib/third_party/EASTL/benchmark/source/BenchmarkSort.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkSort.o lib/third_party/EASTL/benchmark/source/BenchmarkSort.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkString.o: lib/third_party/EASTL/benchmark/source/BenchmarkString.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkString.o lib/third_party/EASTL/benchmark/source/BenchmarkString.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkTupleVector.o: lib/third_party/EASTL/benchmark/source/BenchmarkTupleVector.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkTupleVector.o lib/third_party/EASTL/benchmark/source/BenchmarkTupleVector.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkVector.o: lib/third_party/EASTL/benchmark/source/BenchmarkVector.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/BenchmarkVector.o lib/third_party/EASTL/benchmark/source/BenchmarkVector.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/EASTLBenchmark.o: lib/third_party/EASTL/benchmark/source/EASTLBenchmark.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/EASTLBenchmark.o lib/third_party/EASTL/benchmark/source/EASTLBenchmark.cpp

${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/main.o: lib/third_party/EASTL/benchmark/source/main.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/benchmark/source/main.o lib/third_party/EASTL/benchmark/source/main.cpp

${OBJECTDIR}/lib/third_party/EASTL/source/allocator_eastl.o: lib/third_party/EASTL/source/allocator_eastl.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/source/allocator_eastl.o lib/third_party/EASTL/source/allocator_eastl.cpp

${OBJECTDIR}/lib/third_party/EASTL/source/assert.o: lib/third_party/EASTL/source/assert.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/source/assert.o lib/third_party/EASTL/source/assert.cpp

${OBJECTDIR}/lib/third_party/EASTL/source/fixed_pool.o: lib/third_party/EASTL/source/fixed_pool.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/source/fixed_pool.o lib/third_party/EASTL/source/fixed_pool.cpp

${OBJECTDIR}/lib/third_party/EASTL/source/hashtable.o: lib/third_party/EASTL/source/hashtable.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/source/hashtable.o lib/third_party/EASTL/source/hashtable.cpp

${OBJECTDIR}/lib/third_party/EASTL/source/intrusive_list.o: lib/third_party/EASTL/source/intrusive_list.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/source/intrusive_list.o lib/third_party/EASTL/source/intrusive_list.cpp

${OBJECTDIR}/lib/third_party/EASTL/source/numeric_limits.o: lib/third_party/EASTL/source/numeric_limits.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/source/numeric_limits.o lib/third_party/EASTL/source/numeric_limits.cpp

${OBJECTDIR}/lib/third_party/EASTL/source/red_black_tree.o: lib/third_party/EASTL/source/red_black_tree.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/source/red_black_tree.o lib/third_party/EASTL/source/red_black_tree.cpp

${OBJECTDIR}/lib/third_party/EASTL/source/string.o: lib/third_party/EASTL/source/string.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/source/string.o lib/third_party/EASTL/source/string.cpp

${OBJECTDIR}/lib/third_party/EASTL/source/thread_support.o: lib/third_party/EASTL/source/thread_support.cpp
	${MKDIR} -p ${OBJECTDIR}/lib/third_party/EASTL/source
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lib/third_party/EASTL/source/thread_support.o lib/third_party/EASTL/source/thread_support.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
