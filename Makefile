main: main.cpp
	g++ main.cpp -o Project -lsfml-graphics -lsfml-window -lsfml-system
random_run: Random.cpp
	g++ Random.cpp -o Random -lsfml-graphics -lsfml-window -lsfml-system
	./Random
run:
	./Project