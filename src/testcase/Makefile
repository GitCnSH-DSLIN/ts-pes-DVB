SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

OBJS_NEED = $(wildcard ../*.o)
OBJS_NEED += ../libdesc.a

TARGET	=	test_pat	test_pmt	test_sdt 	\
			test_nit	test_pes 	test_ca_pid_emm \
			test_tdt	test_tot	test_user_channel


all:$(OBJS) $(TARGET)
		
clean:
	-rm -f $(OBJS) $(TARGET)

install:
	-ln -f $(TARGET) $(KBUILD_OUT)


test_pat:test_pat.o  $(OBJS_NEED) 
	$(CC) -o $@  $^ $(CFLAGS) $(LDFLAGS)

test_pmt:test_pmt.o  $(OBJS_NEED) 
	$(CC) -o $@  $^ $(CFLAGS) $(LDFLAGS)

test_sdt:test_sdt.o  $(OBJS_NEED) 
	$(CC) -o $@  $^ $(CFLAGS) $(LDFLAGS)

test_nit:test_nit.o  $(OBJS_NEED) 
	$(CC) -o $@  $^ $(CFLAGS) $(LDFLAGS)

test_pes:test_pes.o  $(OBJS_NEED) 
	$(CC) -o $@  $^ $(CFLAGS) $(LDFLAGS)

test_ca_pid_emm:test_ca_pid_emm.o  $(OBJS_NEED) 
	$(CC) -o $@  $^ $(CFLAGS) $(LDFLAGS)

test_tdt:test_tdt.o  $(OBJS_NEED) 
	$(CC) -o $@  $^ $(CFLAGS) $(LDFLAGS)

test_tot:test_tot.o  $(OBJS_NEED) 
	$(CC) -o $@  $^ $(CFLAGS) $(LDFLAGS)

test_user_channel:test_user_channel.o  $(OBJS_NEED) 
	$(CC) -o $@  $^ $(CFLAGS) $(LDFLAGS)

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)
