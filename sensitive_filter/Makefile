CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = sensitive_filter

all: $(TARGET)

$(TARGET): sensitive_filter.c
	$(CC) $(CFLAGS) -o $(TARGET) sensitive_filter.c

clean:
	rm -f $(TARGET)
