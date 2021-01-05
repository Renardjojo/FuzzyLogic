#Bin
OUTPUT=./bin/exe

#Include path
IDIR=-Iinclude

#exclude from clean
EXCLUDE=

#Cpp version
CPP_VERSION=-std=c++17

#Relase or debug option
CXX?=g++
CC?=gcc
CXX_DEBUG=-Og $(CPP_VERSION) -g -W -Wall -pg -no-pie -MMD -Wno-unknown-pragmas $(IDIR)
CXX_BUILD=-O3 $(CPP_VERSION) -DNDEBUG -MMD -Wno-unknown-pragmas $(IDIR)

C_DEBUG=-Og -g -pg -no-pie -MMD -W -Wall -Wno-unknown-pragmas $(IDIR)
C_BUILD=-O3 -DNDEBUG -MMD -Wno-unknown-pragmas $(IDIR)

#Valgrind flag
#VFLAG=--leak-check=yes
VFLAG=--leak-check=full --show-leak-kinds=all

#Lib
LDLIBS=-lmingw32

#Cpp and C wildcard
SRCPPS=$(wildcard src/*.cpp) 
SRCS=$(wildcard src/*.c) 
OBJS=$(SRCS:.c=.o) $(SRCPPS:.cpp=.o)

all: $(OUTPUT)

multi :
	mkdir -p bin
	make -j all

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CXX) -c $(CXX_DEBUG) $< -o $@

%.o: %.c
	$(CC) -c $(C_DEBUG) $< -o $@

$(OUTPUT): $(OBJS)
	mkdir -p bin
	$(CXX) -pg -no-pie $^ $(LDLIBS) -o $@

run : $(OUTPUT) 
	./$(OUTPUT)
	
#use command of benchmark makefile
benchmarkRun : 
	$(MAKE) -C benchmark -f Makefile run
	
benchmarkCleanAll : 
	$(MAKE) -C benchmark -f Makefile cleanAll

debug:
	echo not implemented yet

buid:
	echo not implemented yet

#debugger. Use "run" to start
gdb :
	make all 
	gdb $(OUTPUT)

#display function stats
gprof :
	gprof $(OUTPUT) gmon.out > output.txt

#display leak
leak :
	make all
	valgrind $(OUTPUT)

#display leak detail
leakFull :
	make all 
	valgrind $(VFLAG) $(OUTPUT)

#function to find word in project
foundPrintf:
	grep -r $(OBJS) "printf"

#TODO Function that create launcher
launcher :
	touch game.desktop
	echo "[Desktop Entry]" >> game.desktop
	echo "Encoding=UTF-8">> game.desktop
	echo "Version=1.0                                     # version of an app.">> game.desktop
	echo "Name[en_US]=game                                # name of an app." >> game.desktop
	echo "GenericName=game                                # longer name of an app." >> game.desktop
	echo "Exec=./bin/game                                 # command used to launch an app." >> game.desktop
	echo "Terminal=false                                  # whether an app requires to be run in a terminal." >> game.desktop
	echo "Icon[en_US]=/opt/yed-3.11.1/icons/yicon32.png   # location of icon file." >> game.desktop
	echo "Type=Application                                # type." >> game.desktop
	echo "Categories=game;                                # categories in which this app should be listed." >> game.desktop
	echo "Comment[en_US]=Game                             # comment which appears as a tooltip. " >> game.desktop

cleanAll:
	rm -f $(OBJS) $(OBJS:.o=.d) $(OUTPUT)
	$(MAKE) -C benchmark -f Makefile cleanAll

#SRC_FILES = $(filter-out src/bar.cpp, $(wildcard src/*.cpp))
clean :
	rm -f $(filter-out $(EXCLUDE) $(EXCLUDE:.o=.d),$(OBJS:.o=.d) $(OBJS))
