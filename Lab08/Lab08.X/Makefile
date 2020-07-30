all: rpg

rpg.o: rpg Lab8_main.c
	gcc -c Lab8_main.c
rpg: Player.c Game.c Lab8_main.c
	gcc -Wall Player.c Game.c Lab8_main.c -o rpg
