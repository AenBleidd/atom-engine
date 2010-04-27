CXX = g++
FLAGS := -Wall -c
#For debug
ifeq ($(DEBUG),TRUE)
	FLAGS +=  -g -DATOM_DEBUG
endif
#For linux
ifeq ($(SYSTEM),UNIX)
	FLAGS +=  -DUNIX
	CORELIB = -lSDL -lGL
endif
#For windows
ifeq ($(SYSTEM),WINDOWS)
	CORELIB = -lSDL -lopengl32
endif
COREPATH = core/obj/
COREOBJ = $(COREPATH)error.o $(COREPATH)gamefs.o $(COREPATH)window.o $(COREPATH)main.o
UTILSPATH = utils/obj/
FSMANFLAGS = -D_FSMANAGER_
FSMANOBJ = $(UTILSPATH)crc32.o $(UTILSPATH)error.o $(UTILSPATH)gamefs.o $(UTILSPATH)fsman.o 

all : prepare atom fsman
.PHONY : all

# Atom Engine
atom : $(COREOBJ)
	$(CXX) $(CORELIB) $(COREOBJ) -o atom-engine-test/atom
$(COREPATH)error.o : core/error.cpp core/error.h preproc.h
	$(CXX) $(FLAGS) core/error.cpp -o $(COREPATH)error.o
$(COREPATH)gamefs.o : core/gamefs.cpp core/gamefs.h preproc.h core/error.h
	$(CXX) $(FLAGS) core/gamefs.cpp -o $(COREPATH)gamefs.o
$(COREPATH)window.o : core/window.cpp core/window.h core/error.h
	$(CXX) $(FLAGS) core/window.cpp -o $(COREPATH)window.o
$(COREPATH)main.o : core/main.cpp preproc.h core/window.h core/error.h
	$(CXX) $(FLAGS) core/main.cpp -o $(COREPATH)main.o

# File System Manager (FSMan)
fsman: $(FSMANOBJ)
	$(CXX) $(FSMANOBJ) -o atom-engine-test/fsman
$(UTILSPATH)crc32.o : utils/crc32.cpp utils/crc32.h
	$(CXX) $(FLAGS) $(FSMANFLAGS) utils/crc32.cpp -o $(UTILSPATH)crc32.o
$(UTILSPATH)error.o : core/error.cpp core/error.h preproc.h
	$(CXX) $(FLAGS) $(FSMANFLAGS) core/error.cpp -o $(UTILSPATH)error.o
$(UTILSPATH)gamefs.o : core/gamefs.cpp core/gamefs.h preproc.h core/error.h utils/crc32.h
	$(CXX) $(FLAGS) $(FSMANFLAGS) core/gamefs.cpp -o $(UTILSPATH)gamefs.o
$(UTILSPATH)fsman.o : utils/fsman.cpp preproc.h core/gamefs.h core/error.h
	$(CXX) $(FLAGS) $(FSMANFLAGS) utils/fsman.cpp -o $(UTILSPATH)fsman.o

# Prepare the system to compiling
prepare :
	-mkdir $(COREPATH)
	-mkdir $(UTILSPATH)
	-mkdir atom-engine-test
# Cleaning the project
.PHONY : clean
clean : 
	-rm $(COREOBJ) $(FSMANOBJ) atom-engine-test/atom atom-engine-test/fsman
