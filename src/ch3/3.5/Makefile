target = 3_5_cond

$(target).out: $(target).c
	$(CC) -O3 -pthread -o $@ $<

.PHONY: clean
clean:
	rm -f $(target).out
