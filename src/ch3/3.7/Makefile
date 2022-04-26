targets = 3_7_1_rwlock-spin.o \
	3_7_2_rwlock-pthreads.out \
	3_7_3_performance-rwlock.out \
	3_7_3_performance-rwlock-wr.out \
	3_7_3_performance-mutex.out \
	3_7_3_performance-empty.out

CFLAGS = -O3
LDFLAGS = -pthread

# 以下の値を変更して再コンパイルすると、ソースコード中の値も変更される。
NUM_THREAD = 4
HOLDTIME = 100
DEFS = -DNUM_THREAD=$(NUM_THREAD) -DHOLDTIME=$(HOLDTIME)

all: $(targets)

3_7_2_rwlock-pthreads.out: 3_7_2_rwlock-pthreads.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

3_7_3_performance-rwlock.out: 3_7_3_performance.c rwlock.c
	$(CC) $(CFLAGS) $(LDFLAGS) -DRWLOCK $(DEFS) $< -o $@

3_7_3_performance-rwlock-wr.out: 3_7_3_performance.c rwlock-wr.c
	$(CC) $(CFLAGS) $(LDFLAGS) -DRWLOCK_WR $(DEFS) $< -o $@

3_7_3_performance-mutex.out: 3_7_3_performance.c mutex.c
	$(CC) $(CFLAGS) $(LDFLAGS) -DMUTEX $(DEFS) $< -o $@

3_7_3_performance-empty.out: 3_7_3_performance.c empty.c
	$(CC) $(CFLAGS) $(LDFLAGS) -DEMPTY $(DEFS) $< -o $@

.PHONY: clean
clean:
	rm -f $(targets)
