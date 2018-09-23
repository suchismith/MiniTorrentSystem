CC = g++
CFLAGS = -Wall -std=c++11 -lssl -lcrypto -pthread
DEPS = client.h
OBJ = client.o 
all: client
%.o: %.cpp $(DEPS)
	$(CC)  -c -o $@ $< $(CFLAGS)

client: $(OBJ)
	$(CC)  -o $@ $^	$(CFLAGS)

