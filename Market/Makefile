CC=g++
CFLAGS=-c -Wall -Wextra -lboost_system -lboost_filesystem
SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=market

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE).out

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	find . -name "*.o" -type f -delete

fclean: clean
	rm -rf $(EXECUTABLE).out

re: fclean all
