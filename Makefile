
CC=gcc
BIN=httpd
OBJ=http.o main.o
FLAGS=#-D_DEBUG_
LDFLAGS=-lpthread
CGI_PATH=sql wwwroot/cgi-bin

.PHONY:all
all:$(BIN) cgi

$(BIN):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) 
	echo "[linking] [$^] to [$@] ... done"
#目录下所有.c文件生成.o文件

%.o:%.c
	$(CC) -c $< $(FLAGS) 
	echo "[compling] [$^] to [$@] ... done"
cgi:
	for i in `echo $(CGI_PATH)`;\
	do\
		cd $$i;\
		make;\
		cd -;\
	done


.PHONY:clean
clean:
	rm -rf $(BIN) $(OBJ) output
	for i in `echo $(CGI_PATH)`;\
	do\
		cd $$i;\
		make clean;\
		cd -;\
	done
	echo "clean project ... done"


.PHONY:output
output:
	mkdir -p output/wwwroot/cgi-bin
	cp -rf log output
	cp -rf conf output
	cp wwwroot/index.html output/wwwroot
	cp wwwroot/cgi-bin/math_cgi output/wwwroot/cgi-bin
	cp sql/insert_cgi output/wwwroot/cgi-bin
	cp sql/select_cgi output/wwwroot/cgi-bin
	cp -rf sql/lib output
	cp httpd output
	cp plugin/ctl_server.sh output/
	echo "output project ... done"


    



