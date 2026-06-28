CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2 -std=c11 -I../../include
AR ?= ar
OBJDIR ?= ../../obj/mytrans
LIB := libmytrans.a
LIB_PATH := $(OBJDIR)/$(LIB)
SRCS := mytrans.c myhash.c
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all clean

all: $(LIB_PATH)

$(LIB_PATH): $(OBJS)
	$(AR) rcs $@ $^

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR)
