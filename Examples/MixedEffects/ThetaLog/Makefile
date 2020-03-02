## -*- Makefile -*-
##
## User: matthew
## Time: Jun 25, 2015 9:07:06 AM
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CCC = c++
CXX = c++
BASICOPTS = -O3 -s -m64
CCFLAGS = -std=c++14
CXXFLAGS = -O3  -std=c++14 $(EXTRA_CXXFLAGS)
CCADMIN = 


# Define the target directories.
TARGETDIR_thetalog=build


all: $(TARGETDIR_thetalog)/thetalog

## Target: thetalog
OBJS_thetalog =  \
	$(TARGETDIR_thetalog)/ThetaLog.o
USERLIBS_thetalog = $(SYSLIBS_thetalog) 
DEPLIBS_thetalog =  
LDLIBS_thetalog = $(USERLIBS_thetalog)


# Link or archive
$(TARGETDIR_thetalog)/thetalog: $(TARGETDIR_thetalog) $(OBJS_thetalog) $(DEPLIBS_thetalog)
	$(LINK.cc) $(CCFLAGS_thetalog) $(CPPFLAGS_thetalog) -o $@ $(OBJS_thetalog) $(LDLIBS_thetalog)


# Compile source files into .o files
$(TARGETDIR_thetalog)/ThetaLog.o: $(TARGETDIR_thetalog) ThetaLog.cpp
	$(COMPILE.cc) $(CCFLAGS_thetalog) $(CPPFLAGS_thetalog) -o $@ ThetaLog.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_thetalog)/thetalog \
		$(TARGETDIR_thetalog)/ThetaLog.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_thetalog)


# Create the target directory (if needed)
$(TARGETDIR_thetalog):
	mkdir -p $(TARGETDIR_thetalog)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

run : ${Target}
	./$(TARGETDIR_thetalog)/thetalog


