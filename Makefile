# Nom du binaire
TARGET = boysay

# Fichier source
SRC = main.c

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -O2

# Règle principale
all: $(TARGET)

# Compilation
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Nettoyer
clean:
	rm -f $(TARGET)

# Installation dans /usr/local/bin
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod 755 /usr/local/bin/$(TARGET)

# Désinstallation
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)
