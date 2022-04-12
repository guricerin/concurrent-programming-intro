targets = 3_3_0_bad-mutex.o 3_3_0_good-mutex.o 3_3_1_spinlock-slow.o 3_3_1_spinlock-fast.o
CFLAGS = -O3
LDFLAGS = -pthread

all: $(targets)

.PHONY: clean
clean:
	rm -rf $(targets)
