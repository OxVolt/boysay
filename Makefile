# ============================================================
# 🧍‍♂️ boysay - Makefile
# A cursed alternative to cowsay, written in C.
# ============================================================

# Nom du binaire
TARGET = boysay

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Répertoires
SRC = main.c
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
SHAREDIR = $(PREFIX)/share/$(TARGET)
ASCIIDIR = $(SHAREDIR)/ASCII

# ============================================================
# Règles de compilation
# ============================================================

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

# ============================================================
# Installation et désinstallation
# ============================================================

install: $(TARGET)
	@echo "🔧 Installing $(TARGET) to $(BINDIR)"
	mkdir -p $(BINDIR)
	cp $(TARGET) $(BINDIR)/

	@echo "📁 Installing ASCII assets to $(ASCIIDIR)"
	mkdir -p $(ASCIIDIR)
	cp -r ASCII/* $(ASCIIDIR)/

	@echo "✅ Installed successfully."

uninstall:
	@echo "🧹 Removing $(TARGET) from system..."
	rm -f $(BINDIR)/$(TARGET)
	rm -rf $(SHAREDIR)
	@echo "✅ Uninstalled."

# ============================================================
# Utilitaires
# ============================================================

re: clean all

.PHONY: all clean install uninstall re
