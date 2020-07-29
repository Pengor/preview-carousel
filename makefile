all: release/main.out

release/main.out: release/ release/vertex_helper.o release/curl_helper.o release/game_list.o release/main.o
	g++ -Wall -o release/main.out -Ilibs/ release/main.o release/game_list.o release/curl_helper.o release/vertex_helper.o -lGL -lSDL2 -lSDL2_image -lSDL2_ttf -lGLEW -lcurl

release/:
	mkdir release/

release/vertex_helper.o: src/vertex_helper.cpp
	g++ -Wall -c src/vertex_helper.cpp -o release/vertex_helper.o

release/curl_helper.o: src/curl_helper.c
	gcc -Wall -c src/curl_helper.c -o release/curl_helper.o

release/game_list.o: src/game_list.cpp
	g++ -Wall -c -Ilibs/ src/game_list.cpp -o release/game_list.o

release/main.o: src/main.cpp
	g++ -Wall -c -Ilibs/ src/main.cpp -o release/main.o

debug: debug/main.out

debug/main.out: debug/ debug/vertex_helper.o debug/curl_helper.o debug/game_list.o debug/main.o
	g++ -Wall -o debug/main.out -Ilibs/ debug/main.o debug/game_list.o debug/curl_helper.o debug/vertex_helper.o -lGL -lSDL2 -lSDL2_image -lSDL2_ttf -lGLEW -lcurl

debug/:
	mkdir debug/

debug/vertex_helper.o: src/vertex_helper.cpp
	g++ -Wall -c -D DEBUG src/vertex_helper.cpp -o debug/vertex_helper.o

debug/curl_helper.o: src/curl_helper.c
	gcc -Wall -g -c -D DEBUG src/curl_helper.c -o debug/curl_helper.o

debug/game_list.o: src/game_list.cpp
	g++ -Wall -g -c -D DEBUG -Ilibs/ src/game_list.cpp -o debug/game_list.o

debug/main.o: src/main.cpp
	g++ -Wall -g -c -D DEBUG -Ilibs/ src/main.cpp -o debug/main.o

clean: 
	rm -r release/ debug/
