# preview-carousel
Submission for Disney Streaming Services Take Home Exercise

# Build & Run Instructions (Linux)
Make sure the following packages are installed by your package manager:
```
curl
SDL2
GLEW
```
Enter the following in the root of the project folder:
```
cd src
gcc -c curl_helper.c
g++ -c -I../libs/ main.cpp
g++ -o main -I../libs/ main.o curl_helper.o -lGL -lSDL2 -lGLEW -lcurl
./main
```
# Build & Run Instructions (Windows)
TBD

