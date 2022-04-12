targets = 3_2_1_cas-no-atomic.s 3_2_1_cas-atomic.s 3_2_2_tas.s

all: $(targets)

%.s: %.c
	$(CC) -O3 -S $<

.PHONY: clean
clean:
	rm -rf *.s
