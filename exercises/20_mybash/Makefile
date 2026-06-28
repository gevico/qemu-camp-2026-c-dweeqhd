CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -O2 -fPIE -Iinclude
LDFLAGS = -pie

BINDIR = bin
OBJDIR = obj
TARGET = $(BINDIR)/mybash

OBJS = \
	$(OBJDIR)/mybash/main.o \
	$(OBJDIR)/myfile/myfile.o \
	$(OBJDIR)/mysed/mysed.o \
	$(OBJDIR)/mytrans/mytrans.o \
	$(OBJDIR)/mytrans/myhash.o \
	$(OBJDIR)/mywc/mywc.o

all: $(TARGET)

$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(OBJDIR)/mybash/main.o: src/mybash/main.c include/common.h | $(OBJDIR)/mybash
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/myfile/myfile.o: src/myfile/myfile.c src/myfile/myfile.h include/common.h | $(OBJDIR)/myfile
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/mysed/mysed.o: src/mysed/mysed.c src/mysed/mysed.h include/common.h | $(OBJDIR)/mysed
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/mytrans/mytrans.o: src/mytrans/mytrans.c src/mytrans/myhash.h include/common.h | $(OBJDIR)/mytrans
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/mytrans/myhash.o: src/mytrans/myhash.c src/mytrans/myhash.h include/common.h | $(OBJDIR)/mytrans
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/mywc/mywc.o: src/mywc/mywc.c src/mywc/mywc.h include/common.h | $(OBJDIR)/mywc
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR) $(OBJDIR)/mybash $(OBJDIR)/myfile $(OBJDIR)/mysed $(OBJDIR)/mytrans $(OBJDIR)/mywc:
	mkdir -p $@

clean:
	rm -rf $(BINDIR) $(OBJDIR)

.PHONY: all clean
