targets = 3_6_1_barrier-spin.out 3_6_2_barrier-pthreads.out
CFLAGS = -O3
LDFLAGS = -pthread

all: $(targets)

%.out: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(targets)
