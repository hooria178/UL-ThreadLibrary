# Target library
lib := libuthread.a
objs := preempt.o uthread.o context.o queue.o sem.o
CC := gcc
CFLAGS := -Wall -Werror -MMD
CFLAGS += -g

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

ifneq ($(V),1)
Q = @
endif

libuthread.a: $(objs)
	@echo "CC $@"
	ar -rcs libuthread.a $(objs)

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

clean:
	$(Q)rm -rf $(lib) $(objs) $(deps)
