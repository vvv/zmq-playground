LDLIBS = -lzmq
CFLAGS = -g -Wall -Wextra

PROGS = ventilator worker sink

all: $(PROGS)

ventilator: helpers.o
worker: helpers.o
sink: helpers.o

-include $(PROGS:%=%.d)

# Generate dependencies
%.d: %.c
	gcc -E -MM $(CPPFLAGS) $< |\
 sed -E 's%^(.+)\.o:%$(@D)/\1.d $(@D)/\1.o:%' >$@

.PHONY: mostlyclean
mostlyclean:
	rm -f $(PROGS:%=%.d) $(PROGS:%=%.o) helpers.o
	rm -rf $(PROGS:%=%.dSYM)

.PHONY: clean
clean: mostlyclean
	rm -f $(PROGS)
