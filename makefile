# This makefile automatically adds all directories and sub-directories under the 'source' directory as c-source folders, cpp-source folders and include folders.
# If you want to exclude one or more folders from this automated process then add the folders to the EXCLUDEDIRS variable.
# You can manually add source and include files below, libraries and library folders have to be added manually.

EXE := Subdivision
SRCAPP := source/Applications/SubdivisionApp.cpp

EXCLUDESRCS := 
EXCLUDEDIRS := source/Window source/from_school/Collision/mains source/Applications

SRCS := source/Window/glad/src/glad.c $(SRCAPP)
INCLUDE := -Isource/Window/glfw-3.2.1.bin.WIN64/include -Isource/Window/glad/include -Isource/Window

LDIR := -Lsource/Window/glfw-3.2.1.bin.WIN64/lib-mingw-w64
LIBS := -lglfw3 -lopengl32 -lgdi32 -lcomdlg32 -lole32

########## DO NOT EDIT ANYTHING BELOW THIS ##########
########## DO NOT EDIT ANYTHING BELOW THIS ##########
########## DO NOT EDIT ANYTHING BELOW THIS ##########

# Constants
BIN := build
SRCROOT = source

# Windoes shell script to find all subdirectories in SRCROOT 
define FINDSRCDIRS
SETLOCAL EnableDelayedExpansion
FOR /F "delims=" %%F IN ('DIR /A:D /B /S $(SRCROOT)') DO (
    SET _dir=%%F
    SET _res=!_dir:%cd%\=!
    echo !_res!
)
endef

# Command line arguments
ifndef OL
OL := -O0
endif

# Store path to source files, in a convenient format
SRCDIRS := $(patsubst %/,%,$(dir $(SRCS)))
# Add source root as source directory
SRCDIRS += $(SRCROOT)
# Add all folders below source root as source directories, in a convenient format. Folders to and below the ones specified in the EXCLUDEDIRS variable are ignored
SRCDIRS += $(subst \,/,$(shell $(FINDSRCDIRS)))
# Remove excluded folders from source directories
SRCDIRS := $(filter-out $(addsuffix %,$(EXCLUDEDIRS)),$(SRCDIRS))

# Find all source files in all source directories
SRCS += $(wildcard $(SRCDIRS:%=%/*.c))
SRCS += $(wildcard $(SRCDIRS:%=%/*.cpp))
SRCS += $(wildcard $(SRCDIRS:%=%/*.cc))
# Remove excluded files from source files
SRCS := $(filter-out $(EXCLUDESRCS),$(SRCS))
# Set all source directories as include folders, in a valid format
INCLUDE += $(SRCDIRS:%=-I%) -MMD -MP

# TODO Remember to check for duplicates and abort if found
# Create object and dependency files from source files below the binary folder
OBJS := $(filter %.o,$(SRCS:%.c=$(BIN)/%.o) $(SRCS:%.cpp=$(BIN)/%.o) $(SRCS:%.cc=$(BIN)/%.o))
DEPS := $(OBJS:.o=.d)

# List of binary directories
BINDIRS := $(BIN) $(patsubst %,build/%,$(dir $(SRCS)))

# Dummy variable that's never used. Evaluated immidiately which creates all the needed binary directories
create-output-directories := $(shell for %%f in ($(subst /,\,$(BINDIRS))) do if not exist %%f (mkdir %%f))

$(BIN)/$(EXE): $(OBJS)
	g++ -o $@ $^ $(LDIR) $(LIBS)

$(BIN)/%.o: %.cpp
	g++ -c $(OL) -o $@ $< $(INCLUDE)

$(BIN)/%.o: %.cc
	g++ -c $(OL) -o $@ $< $(INCLUDE)

$(BIN)/%.o: %.c
	g++ -c $(OL) -o $@ $< $(INCLUDE)

.PHONY: clean
clean:
	rmdir /Q /S $(BIN)

-include $(DEPS)

Resources for future improvement of this makefile:
http://aegis.sourceforge.net/auug97.pdf
http://uploads.mitechie.com/books/Managing_Projects_with_GNU_Make_Third_Edition.pdf