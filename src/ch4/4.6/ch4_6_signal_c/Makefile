targets = signal_c.out
CFLAGS = -O3
LDFLAGS = -pthread

all: $(targets)

%.out: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(targets)
