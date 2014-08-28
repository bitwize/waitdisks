CC=cc

all: waitdisks

waitdisks: waitdisks.o waitdisks_util.o
	$(CC) waitdisks.o waitdisks_util.o -o $@
.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

