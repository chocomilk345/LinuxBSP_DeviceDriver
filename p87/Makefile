CC=gcc

TARGET=main

LDFLAGS=

all : $(TARGET)

$(TARGET):$(TARGET).o
	$(CC) -o $@ $< $(LDFLAGS)

%.o:%.c
	$(CC) -c -o $@ $<

clean:
	rm -f *.o $(TARGET)
