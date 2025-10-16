TARGET = boysay
SRC    = main.c
CC     = gcc
CFLAGS = -Wall -Wextra -O2

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod 755 /usr/local/bin/$(TARGET)

uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

run: $(TARGET)
	./$(TARGET) $(MSG)

