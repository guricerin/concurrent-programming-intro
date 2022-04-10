targets = 2_1_1_pthreads.out 2_1_2_pthreads.out 2_2_no-volatile.s 2_2_volatile.s
CFLAGS = -O3
LDFLAGS = -pthread

all: $(targets)

%.out: %.c
	$(CC) -O3 $(LDFLAGS) -o $@ $<

%.s: %.c
	$(CC) -O3 -S $<

.PHONY: clean
clean:
	rm -f $(targets)
