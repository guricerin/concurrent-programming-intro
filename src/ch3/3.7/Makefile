targets = 3_7_1_rwlock-spin.o
CFLAGS = -O3
LDFLAGS = -pthread

all: $(targets)

%.out: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(targets)
