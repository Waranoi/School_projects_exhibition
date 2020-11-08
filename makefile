# This makefile automatically adds all directories and sub-directories under the 'source' directory as c-source folders, cpp-source folders and include folders.
# If you want to exclude one or more folders from this automated process then add the folders to the EXCLUDE variable.
# You can manually add source and include files below, libraries and library folders have to be added manually.

EXCLUDE := source/Window

CSRCS := source/Window/glad/src/glad.c
CXXSRCS := 
INCLUDE := -Isource/Window/glfw-3.2.1.bin.WIN64/include -Isource/Window/glad/include -Isource/Window

LDIR := -Lsource/Window/glfw-3.2.1.bin.WIN64/lib-mingw-w64
LIBS := -lglfw3 -lopengl32 -lgdi32 -lcomdlg32 -lole32

########## DO NOT EDIT ANYTHING BELOW THIS ##########
########## DO NOT EDIT ANYTHING BELOW THIS ##########
########## DO NOT EDIT ANYTHING BELOW THIS ##########

# Constants
EXE := Subdivision
BIN := build
SRCROOT = source

# Command line arguments
ifndef OL
OL := -O0
endif

# Store path to source files, in a convenient format
SRCDIRS := $(patsubst %/,%,$(dir $(CSRCS)) $(dir $(CXXSRCS)))
# Add source root as source directory
SRCDIRS += $(SRCROOT)
# Add all folders below source root as source directories, in a convenient format. Folders to and below the ones specified in the EXCLUDE variable are ignored
SRCDIRS += $(subst \,/,$(patsubst $(shell CHDIR )\\%,%,$(shell DIR /A:D /B /S $(SRCROOT))))
# Remove excluded folders from source directories
SRCDIRS := $(filter-out $(addsuffix %,$(EXCLUDE)),$(SRCDIRS))

# Find all source files in all source directories
CSRCS += $(wildcard $(SRCDIRS:%=%/*.c))
CXXSRCS += $(wildcard $(SRCDIRS:%=%/*.cpp))
CXXSRCS += $(wildcard $(SRCDIRS:%=%/*.cc))
# Set all source directories as include folders, in a valid format
INCLUDE += $(SRCDIRS:%=-I%) -MMD -MP

# TODO Remember to check for duplicates and abort if found
# Create object and dependency files from source files below the binary folder
OBJS := $(CSRCS:%.c=$(BIN)/%.o) $(filter %.o,$(CXXSRCS:%.cpp=$(BIN)/%.o) $(CXXSRCS:%.cc=$(BIN)/%.o))
DEPS := $(OBJS:.o=.d)

# List of binary directories
BINDIRS := $(BIN) $(patsubst %,build/%,$(dir $(CSRCS)) $(dir $(CXXSRCS)))

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
	$(shell rmdir /Q /S "$(BIN)" )

-include $(DEPS)

# Resources for future improvement of this makefile:
# http://aegis.sourceforge.net/auug97.pdf
# http://uploads.mitechie.com/books/Managing_Projects_with_GNU_Make_Third_Edition.pdf