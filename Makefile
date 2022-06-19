SOURCES = main.c \
	#comunicacao.c \
	ctrl.c \
	interface.c \

OBJS = $(SOURCES:.c=.o)

TARGET = pmr3401

.PHONY: build
build: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $@ $(OBJS)

%.o : %.c
	gcc -Wall -g -c $< -o $@

.PHONY: clean
clean:
	del $(OBJS) $(TARGET).exe
#	For non Windows users:
#	rm -f $(OBJS) $(TARGET)
