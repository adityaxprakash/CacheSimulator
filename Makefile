CC = g++
CFLAGS = -Wall -Wextra --pedantic -g

SRCDIR = src
INCDIR = include
OBJDIR = obj

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
TARGET = cacheSim

.PHONY: all clean directories

# Default target
all: directories $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

directories:
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)