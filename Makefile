program=main.cpp
o=Game
O=1
SFML=/home/Elyxin169/Documents/SFML-2.5.1/build1
debug=Wall #if set to g, it will be debug
# custom libs
compile: $(program)
	g++ -o $(o) -O$(O) -$(debug) $(program) /home/Elyxin169/Documents/SFML-2.5.1-sources/SFML-2.5.1/build1/lib/libsfml-graphics-s.a /home/Elyxin169/Documents/SFML-2.5.1-sources/SFML-2.5.1/build1/lib/libsfml-window-s.a /home/Elyxin169/Documents/SFML-2.5.1-sources/SFML-2.5.1/build1/lib/libsfml-audio-s.a /home/Elyxin169/Documents/SFML-2.5.1-sources/SFML-2.5.1/build1/lib/libsfml-system-s.a -static-libgcc -static-libstdc++ -ludev -lGL -lXext -lXrandr -lX11 -lfreetype
	# -lX11
	# -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -L $(SFML)/lib 
	# --static
run: compile
	./$(o)