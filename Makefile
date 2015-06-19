CFLAGS    = -g  -Wall -Iinclude \

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

TARGET=test

all: $(TARGET)
	@echo "***********************************************"
	@echo "** ALL :   Test       **"
	@echo "***********************************************"
	@echo

clean:
	rm -f $(TARGET) *.o

test:$(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
$(OBJS) : $(SRCS)

