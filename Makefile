# set compiler and compile options
EXEC = rpimp3
CXX = g++                                # use the GNU C++ compiler
OPTS = -O0 -Wall -std=c++0x -Wno-write-strings \
-DUSE_OPENGL -DUSE_EGL -DIS_RPI -DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX \
-D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE \
-DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST \
-DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi -I/opt/vc/include/ \
-I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux \
-Wno-deprecated-declarations
CFLAGS = $(OPTS)                         # add compile flags
LDFLAGS = -L/opt/vc/lib -lGLESv2 -lEGL -lbcm_host      # specify link flags here

# set a list of directories
OBJDIR = ./obj
BINDIR = ./bin
SRCDIR = ./src

# set the include folder where the .h files reside
CFLAGS += -I$(SRCDIR)

# add here the source files for the compilation
SOURCES = rpimp3.cpp

# create the obj variable by substituting the extension of the sources
# and adding a path
_OBJ = $(SOURCES:.cpp=.o)
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJ)
	$(CXX) -o $(BINDIR)/$(EXEC) $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

test: $(BINDIR)/$(EXEC)
	$(BINDIR)/$(EXEC)

clean:
	rm -vf $(BINDIR)/$(EXEC) $(OBJ)
