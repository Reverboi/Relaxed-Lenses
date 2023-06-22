# Relaxed-Lenses

## Informazioni generali
Relaxed-Lenses è un ambiente di simulazione scritto in C++ dotato di una serie di funzioni utili per progettare e ottimizzare un sistema ottico telecentrico.
Il progetto è nato con lo scopo ben preciso di individuare un doppietto acromatico da affiancare a un ottica principale con una geometria già stabilita, per questo motivo è già implememntato un sistema rudimentale per il calcolo della dispersione cromatica.

## Mathematica
Nel progetto trovate anche una cartella contenente un notebook scritto per l'ambente di calcolo Mathematica.
Il suo utilizzo è da considerarsi del tutto facoltativo anche perchè non si tratta di un software gratuito.
Il suo scopo è semplicemente quello di prendere una serie di punti nel piano e restituire un polinomio i cui coefficienti possono poi essere utilizzati per inizializzare una lente nel programma vero e proprio.
Si tratta insomma di un progetto ausiliario indipendente e ripeto facoltativo rispetto a R-L.

## Compilare ed eseguire il progetto
Questo progetto è pensato per essere compilato con CMAKE.
Per farlo segui queste semplici istruzioni:
1. Se non lo hai già fatto installa CMAKE sul tuo computer.
Per esempio su Ubuntu e simili è sufficiente il comando `sudo apt install cmake`
2. Crea una nuova cartella di nome *build* all'interno del progetto (accanto a dove trovi il file CMakeLists.txt per intenderci). Puoi farlo col comando `mkdir build`
3. Naviga all'interno di questa ed esegui il comando `cmake ..`
questo genererà nella cartella in cui ti trovi tutti i necessari file per la compilazione del progetto cercando il file di nome CmMakeLists.txt nella cartella specificata con `..` che è la cartella madre di dove ti trovi ora ovvero il *root* del progetto.
4. Finalmente, senza uscire da build esegui il comando `make`.
	Su sistemi Linux questo eseguirà la compilazionee ti lascerà con un eseguibile di nome *Relaxed-Lenses* che puoi eseguire digitando `./Relaxed-Lenses`.
	Su Windows invece genererà una Visual Studio Solution da cui compilare e lanciare effettivamenete il programma.
	In Visual Studio potrebbe essere necessario impostare Relaxed-Lenses come "Startup Project" affinchè tutto funzioni.
## Esempio di formattazione file di inizializzazione di una curva polinomiale
Come già accennato è possibile inizializzare una curva free-form specificandone i parametri all'interno di un file di testo formattato come segue:
```
A: 35                                      // il "raggio" della lente (semi-apertura)
H: 0.0                                     // posizione lungo l'asse ottico (riferita alle estremità)
Q: 1.51269, 3.97321, -0.335263, 0.398982   // un numero arbitrario di coeficienti polinomiali (tutti pari)
```
Buon otticazione.