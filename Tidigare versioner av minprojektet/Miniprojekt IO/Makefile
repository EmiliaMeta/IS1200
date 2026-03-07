# Kompilator och flaggor
CC     = gcc
CFLAGS = -Wall -Wextra -std=c11

# Projektets filer
OBJS = main.o objects.o systems.o textstring.o menu.o map.o io.o boot_surprise.o
TARGET = game

# Standardmål: bygg spelet
all: $(TARGET)

# Länka ihop alla objektfiler till en körbar fil
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Regler för att bygga .o från .c och .S
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@


# Rensa bort binärer och objektfiler
clean:
	del /Q *.o game 2>nul || rm -f *.o game

# Hjälpmål för att köra spelet direkt efter build
run: $(TARGET)
	./$(TARGET)


