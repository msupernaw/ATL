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
CCFLAGS = -std=c++11
CXXFLAGS =-O3 -std=c++11
CCADMIN = 


# Define the target directories.
TARGETDIR_orangetree=build


all: $(TARGETDIR_orangetree)/orangetree

## Target: orangetree
OBJS_orangetree =  \
	$(TARGETDIR_orangetree)/OrangeTree.o
USERLIBS_orangetree = $(SYSLIBS_orangetree) 
DEPLIBS_orangetree =  
LDLIBS_orangetree = $(USERLIBS_orangetree)


# Link or archive
$(TARGETDIR_orangetree)/orangetree: $(TARGETDIR_orangetree) $(OBJS_orangetree) $(DEPLIBS_orangetree)
	$(LINK.cc) $(CCFLAGS_orangetree) $(CPPFLAGS_orangetree) -o $@ $(OBJS_orangetree) $(LDLIBS_orangetree)


# Compile source files into .o files
$(TARGETDIR_orangetree)/OrangeTree.o: $(TARGETDIR_orangetree) OrangeTree.cpp
	$(COMPILE.cc) $(CCFLAGS_orangetree) $(CPPFLAGS_orangetree) -o $@ OrangeTree.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_orangetree)/orangetree \
		$(TARGETDIR_orangetree)/OrangeTree.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_orangetree)


# Create the target directory (if needed)
$(TARGETDIR_orangetree):
	mkdir -p $(TARGETDIR_orangetree)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

run : ${Target}
	./$(TARGETDIR_orangetree)/orangetree

