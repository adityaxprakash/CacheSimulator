CC = gcc
CFLAGS = -Wall -Wextra --pedantic

SRCDIR = src
INCDIR = include
OBJDIR = obj

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
TARGET = cacheSim

.PHONY: all clean directories

all: $(TARGET)

$(TARGET): directories $(OBJS)
	$(CC) $(OBJS) -o $@ -lstdc++

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

directories:
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR) $(TARGET)