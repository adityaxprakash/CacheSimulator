CC = g++
CFLAGS = -Wall -Wextra --pedantic -g

SRCDIR = src
INCDIR = include

SRCS = $(wildcard $(SRCDIR)/*.cpp)

OBJS = $(SRCS:.cpp=.o)

TARGET = cacheSim

# Default target
all: $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) $(TARGET) csim