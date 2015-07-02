CFLAGS    = -g  -Wall -Iinclude -Idescriptors/include \

SRCS = $(wildcard *.c)
SRCS += $(wildcard ./descriptors/*.c ./test_program/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
TARGET = $(wildcard ./test_program/*.c)


CUR_DIR = $(shell pwd)
OBJDIR_O = $(CUR_DIR)/objdir_o
OBJDIR_D = $(CUR_DIR)/objdir_d

define MKDIR
@echo 'making directory $(OBJDIR_D)...'
if [ ! -d $(OBJDIR_D) ];then mkdir $(OBJDIR_D);fi
@echo 'making directory $(OBJDIR_O)...'
if [ ! -d $(OBJDIR_O) ];then mkdir $(OBJDIR_O);fi
@echo ''
endef

TARGET=test_sdt

all: $(TARGET)
	@echo "***********************************************"
	@echo "** ALL :   Test       **"
	@echo "***********************************************"
	@echo

clean:
	rm -f $(TARGET) *.o ./descriptors/*.o

test_sdt:$(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
$(OBJS) : $(SRCS)

