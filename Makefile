# Si clang non trouvé, possible d'utiliser gcc
CC=clang
CFLAGS=-g -Wall -Wextra -Werror -gdwarf-4

EXEC=client-tutoriel client-interactif client-projetX client-projetX client-crypteMove client-BayOfPigs client-crypteSeq client-Northwoods client-lostcause client-OneMillion client-PlanB

# Utilisé uniquement si exécution sur Caseine.
# Pour changer le programme lancé par Caseine, modifiez la ligne ci-dessous
MAIN=client-interactif

all: $(EXEC)

client-interactif: client-interactif.o client.o

client-tutoriel: client-tutoriel.o client.o
client-tutorielFORT: client-tutorielFORT.o client.o
client-projetX: client-projetX.o client.o
client-crypteMove: client-crypteMove.o client.o
client-BayOfPigs: client-BayOfPigs.o client.o
client-crypteSeq: client-crypteSeq.o client.o
client-Northwoods: client-Northwoods.o client.o
client-lostcause: client-lostcause.o client.o
client-OneMillion: client-OneMillion.o client.o
client-PlanB: client-PlanB.o client.o
client.o: client.c client.h

clean:
	rm -f *.o

clear:
	rm -f $(EXEC)

main: $(MAIN)
	cp $< $@

.PHONY: clear clean main