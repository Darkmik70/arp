all: main server reader writer
	
clean-logs:
	rm build/log/reader_0/* build/log/reader_1/*
clean:
	rm build/main build/server build/reader build/writer
main:
	gcc main.c -o build/main
server:
	gcc server.c -o build/server
reader:
	gcc reader.c -o build/reader
writer:
	gcc writer.c -o build/writer