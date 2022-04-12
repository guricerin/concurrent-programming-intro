targets = 3_2_1_cas.s

all: $(targets)

%.s: %.c
	$(CC) -O3 -S $<

.PHONY: clean
clean:
	rm -rf *.s
