CFLAGS=-g -Wall -W -std=gnu99 -fvisibility=hidden
LDFLAGS=-Wl,--as-needed -L. -Wl,-R. -Wl,-Bstatic -llol -Wl,-Bdynamic

C_SOURCES := $(wildcard *.c)
D_FILES := $(patsubst %.c,%.d,$(C_SOURCES))

.PHONY: clean all generate-deps help
all: generate-deps liblol.a liblol.so lol-example lol-test

generate-deps: $(D_FILES)

lol-example: example.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

lol-test: test.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

liblol.so: lol.c
	$(CC) -fPIC -shared $^ -o $@ $(CFLAGS)

liblol.a: lol.o
	$(AR) rcs $@ $^

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(wildcard *.d)

format:
	find . -type f -name "*.[ch]" | xargs clang-format -i

help:
	@$(MAKE) --print-data-base --question |       						  \
	awk '/^[^.%][-a-zA-Z0-9_]*:/ {print substr($$1, 1, length($$1)-1)}' | \
	sort |  									                          \
	grep -v "Makefile" |												  \
	pr --omit-pagination --width=80 --columns=4

clean:
	rm -f *.o *.d lol-example lol-test liblol.a liblol.so
