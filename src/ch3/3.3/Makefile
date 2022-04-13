targets = 3_3_0_bad-mutex.o 3_3_0_good-mutex.o 3_3_1_spinlock-slow.o 3_3_1_spinlock-fast.o 3_3_2_pthreads-mutex.out
CFLAGS = -O3
LDFLAGS = -pthread

all: $(targets)

%.out: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf $(targets)
