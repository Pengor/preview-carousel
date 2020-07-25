all: release/main.out

release/main.out: release/ release/curl_helper.o release/main.o
	g++ -Wall -o release/main.out -Ilibs/ release/main.o release/curl_helper.o -lGL -lSDL2 -lGLEW -lcurl

release/:
	mkdir release/

release/curl_helper.o: src/curl_helper.c
	gcc -Wall -c src/curl_helper.c -o release/curl_helper.o

release/main.o: src/main.cpp
	g++ -Wall -c -Ilibs/ src/main.cpp -o release/main.o

debug: debug/main.out

debug/main.out: debug/ debug/curl_helper.o debug/main.o
	g++ -Wall -o debug/main.out -Ilibs/ debug/main.o debug/curl_helper.o -lGL -lSDL2 -lGLEW -lcurl

debug/:
	mkdir debug/

debug/curl_helper.o: src/curl_helper.c
	gcc -Wall -g -c src/curl_helper.c -o debug/curl_helper.o

debug/main.o: src/main.cpp
	g++ -Wall -g -c -Ilibs/ src/main.cpp -o debug/main.o

clean: 
	rm -r release/ debug/
