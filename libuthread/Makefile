# Target library
lib := libuthread.a
objs := preempt.o uthread.o context.o queue.o sem.o
CC := gcc
CFLAGS := -Wall -Werror
CFLAGS += -g
DEPFLAGS = -MMD -MF $(@:.o=.d)

ifneq ($(V),1)
Q = @
endif

all: $(lib)

$(lib): $(objs)
	$(Q)$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

clean:
	@echo "clean"
	$(Q)rm -f $(targets) $(objs) $(lib) *.d
