# Compilador
CC := gcc

# Nome do executavel
TARGET = pp.exe

# Arquivos fonte .c
SOURCES = main.c \
	tasks.c \
	queue.c \
	list.c \
	timers.c \
	event_groups.c \
	heap_4.c \
	port.c \

# Diretorios de busca para #include
INCLUDE_DIRS := -I.
INCLUDE_DIRS += -I./include

# Flags para a compilacao (geracao dos arquivos .o)
CFLAGS := -Wall

# Flags para o linker (.o's para executavel)
LDFLAGS := -lWinmm

OBJS = $(SOURCES:.c=.o)

.PHONY: build
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

.PHONY: clean
clean:
	del $(OBJS) $(TARGET)
#	For non Windows users:
#	rm -f $(OBJS) $(TARGET)