CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2 -std=c11 -I../../include
BINDIR ?= ../../bin
OBJDIR ?= ../../obj/mybash
TARGET := mybash
TARGET_PATH := $(BINDIR)/$(TARGET)
SRCS := main.c
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
LDLIBS ?= -L../../obj/mysed -L../../obj/mywc -L../../obj/mytrans -L../../obj/myfile -lmysed -lmywc -lmytrans -lmyfile

.PHONY: all clean

all: $(TARGET_PATH)

$(TARGET_PATH): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) -o $@ $^ $(LDLIBS)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(TARGET_PATH)
