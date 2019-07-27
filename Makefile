
CFLAGS = -Wall -Wextra -Isrc

VPATH = src t

TESTS=io-test compat

OBJS = src/sacio.o src/geodesic.o src/timespec.o src/time64.o \
			 src/strip.o src/compat.o src/header_map.o

LIB = libsacio-bsd.a

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f $(OBJS) src/*~ t/*~ core $(LIB) t/*.o $(TESTS) t/*.tmp

test: $(TESTS)
	./io-test
	./compat
	diff t/test_io_big.sac t/test_io_big.sac.tmp
	diff t/test_io_small.sac t/test_io_small.sac.tmp
	diff t/test_uneven_big.sac t/test_uneven_big.sac.tmp
	diff t/test_uneven_small.sac t/test_uneven_small.sac.tmp
	diff t/test_spec_big.sac t/test_spec_big.sac.tmp
	diff t/test_spec_small.sac t/test_spec_small.sac.tmp

io-test: t/io-test.o $(LIB)
compat: t/compat.o $(LIB)

src/header_map.c: src/header_map.txt
	gperf -t -T --output-file=$@ -N sac_keyword_to_header $<
