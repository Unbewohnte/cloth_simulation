all:
	g++ -O2 -Wall -Werror src/*.cpp lib/libSDL2.a -o cloth_sim

clean:
	rm -f cloth_sim