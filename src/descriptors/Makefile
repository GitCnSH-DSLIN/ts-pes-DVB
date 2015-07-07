SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

TARGET = libdesc.a

all:libdesc.a
	-ln -f $(TARGET) ../

clean:
	-rm -f $(OBJS) $(TARGET)

install:


%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)

libdesc.a:$(OBJS)
	${AR} -r -o $@ $^	
