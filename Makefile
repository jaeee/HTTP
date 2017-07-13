
CC=gcc
BIN=httpd
OBJ=http.o main.o

LDFLAGS=-lpthread

$(BIN):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) 
#目录下所有.c文件生成.o文件

%.o:%.c
	$(CC) -c $< 

.PHONY:clean
clean:
	rm -f $(BIN) $(OBJ)
