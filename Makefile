all:
	g++ -O3 main.cpp header.hpp -o mz -I/usr/local/include -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL3