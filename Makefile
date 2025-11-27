CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -I./src
LDFLAGS = -lncurses
SRCDIR = src
BUILDDIR = build
BINDIR = bin
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
TARGET = $(BINDIR)/ajedrez_ncurses

.PHONY: all clean dirs

all: dirs $(TARGET)

dirs:
	mkdir -p $(BUILDDIR) $(BINDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILDDIR) $(BINDIR)
