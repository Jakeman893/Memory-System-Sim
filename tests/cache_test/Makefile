SRC_DIR = ../../src/
A_SRC = cache.c
A_HEAD = cache.h
A_OBJS = $(A_SRC_LOC:.c=.o)
A_SRC_LOC = $(addprefix $(SRC_DIR), $(A_SRC))
A_H_LOC = $(addprefix $(SRC_DIR), $(A_HEAD))

all: $(A_SRC_LOC) cache.unittest

%.o: %.c
	g++ -g -Wall -c -o $@ $<

cache.unittest: $(A_OBJS) ../../src/cache.h
	g++ -g cache_unittest.cpp -lgtest -lgtest_main -lpthread $^ -o $@

clean:
	rm cache.unittest
	rm $(A_OBJS)
