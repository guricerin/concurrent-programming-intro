targets = 2_1_1_pthreads.out
CFLAGS = -O3
LDFLAGS = -pthread

all: $(targets)

%.out: %.c
	$(CC) -O3 $(LDFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(targets)
