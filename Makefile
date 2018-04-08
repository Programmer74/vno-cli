INC := inc

CC=g++
CFLAGS=-c -Wall -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0
LDFLAGS=
SOURCES=src/main.cpp src/utils.cpp src/init.cpp src/commit.cpp src/checkout.cpp src/status.cpp src/log.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=vno-cli

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) -I$(INC) $(CFLAGS) $< -o $@

clean:
	rm -rf src/*.o $(EXECUTABLE)
