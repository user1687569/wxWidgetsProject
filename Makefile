# Copyright (c) 2022 user1687569

CXX = g++

BIN_DIR	= bin
INC_DIR = inc
OBJ_DIR = obj
RES_DIR = res
SRC_DIR = src

INC_WXWIDGETS 	= E:\wxWidgets-3.1.5\include
INC_WXWIDGETS2 	= E:\wxWidgets-3.1.5\lib\gcc_lib\mswu
LIB_WXWIDGETS 	= E:\wxWidgets-3.1.5\lib\gcc_lib

VPATH = $(SRC_DIR)

WXDEFS = -D __WXMSW__ -D _UNICODE
INC_PATH = -I $(INC_WXWIDGETS) -I $(INC_WXWIDGETS2) -I$(RES_DIR)
LIB_PATH = -L $(LIB_WXWIDGETS)
LIBS =	-lwxmsw31u_core -lwxbase31u -lwxtiff -lwxjpeg -lwxpng -lwxzlib -lwxregexu -lwxexpat 	\
		-lkernel32 -luser32 -lgdi32 -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme

CXXFLAGS = -W -Wall -O2 -mthreads $(WXDEFS) $(INC_PATH)

APP_NAME := animationTest
TARGET := $(BIN_DIR)/$(APP_NAME).exe
OBJS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(wildcard $(SRC_DIR)/*.cpp)))	\
		$(OBJ_DIR)/animationTest_rc.o

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) -mthreads -Wl,--subsystem,windows -mwindows  $(LIB_PATH) $(LIBS) 

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<

$(OBJ_DIR)/animationTest_rc.o: $(RES_DIR)/wxWidgets.rc
	windres	-i$<  -o$@ $(WXDEFS) $(INC_PATH)

.PHONY: all clean cleanObj
clean: 
	-if exist $(OBJ_DIR)\*.o del $(OBJ_DIR)\*.o
	-if exist $(OBJ_DIR)\*.d del $(OBJ_DIR)\*.d
	-if exist $(BIN_DIR)\*.exe del $(BIN_DIR)\*.exe

cleanObj:
	-if exist $(OBJ_DIR)\*.o del $(OBJ_DIR)\*.o
	-if exist $(OBJ_DIR)\*.d del $(OBJ_DIR)\*.d
