SUBDIRS = descriptors testcase

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

 
all:$(OBJS)
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i ; done

install:
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i install ; done

clean:
	@set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean ; done
	rm -rf *.o
	rm -rf *.a

%.o:%.c
	@echo --------------------------
	@echo $(OBJS):$(SRCS)
	$(CC) -o $@ -c $< $(CFLAGS)
