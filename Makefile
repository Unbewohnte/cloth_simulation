all:
	g++ -lSDL2 -O2 -Wall -Werror src/*.cpp -o cloth_sim

clean:
	rm -f cloth_sim