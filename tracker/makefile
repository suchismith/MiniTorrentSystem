CC = g++
CFLAGS = -Wall -std=c++1z -pthread 
DEPS = tracker.h
OBJ = tracker.o 
all: tracker
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

tracker: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

