
CFLAGS = -Wall -Wextra -Isrc

VPATH = src t

OBJS = src/sacio.o src/geodesic.o src/timespec.o src/time64.o src/strip.o

LIB = libsacio-bsd.a

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f $(OBJS) *~ core $(LIB)

test: io-test
	./io-test

io-test: t/io-test.o $(LIB)
