CC = g++
CFLAGS = -std=c++17 -Wall -Wextra

SRC_FILES = main.cpp markov_model.cpp
HEADER_FILE = markov_model.hpp

testApp: $(SRC_FILES) $(HEADER_FILE)
	$(CC) $(CFLAGS) $(SRC_FILES) -o testApp

clean:
	rm -f testApp
