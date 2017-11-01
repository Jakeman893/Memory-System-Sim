SRC_DIR = ../../src/
A_SRC = memsys.c cache.c dram.c
A_HEAD = memsys.h cache.h dram.h
A_OBJS = $(A_SRC_LOC:.c=.o)
A_SRC_LOC = $(addprefix $(SRC_DIR), $(A_SRC))
A_H_LOC = $(addprefix $(SRC_DIR), $(A_HEAD))

all: $(A_SRC_LOC) multicache.unittest

%.o: %.c
	g++ -g -Wall -c -o $@ $<

multicache.unittest: $(A_OBJS) ../../src/cache.h ../../src/memsys.h ../../src/dram.h
	g++ -g multicache_unittest.cpp -lgtest -lgtest_main -lpthread $^ -o $@

clean:
	rm multicache.unittest
	rm $(A_OBJS)
