
CFLAGS = -Wall -Wextra

OBJS = sacio.o geodesic.o timespec.o time64.o strip.o

LIB = libsacio-bsd.a

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(OBJS) *~ core $(LIB)
