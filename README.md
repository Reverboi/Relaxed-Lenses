#Relaxed-Lenses

Questo progetto è pensato per essere compilato con CMAKE.
Per farlo segui queste semplici istruzioni:
0. Se non lo hai già fatto installa CMAKE sul tuo computer.
Per esempio su Ubuntu e simili è sufficiente il comando `sudo apt install cmake`.
1. Crea una nuova cartella di nome 'build' all'interno del progetto (accanto a dove trovi il file CMakeLists.txt per intenderci). Puoi farlo col comando `mkdir build`.
2. Naviga all'interno di questa ed esegui il comando `cmake ..`, questo genererà nella cartella in cui ti trovi tutti i necessari file per la compilazione del progetto cercando il file di nome CmMakeLists.txt nella cartella specificata con `..` che è la cartella madre di dove ti trovi ora ovvero il *root* del progetto.
3. Finalmente, senza uscire da build esegui il comando `make`, questo eseguirà la compilazione vera e propria e ti lascerà con un eseguibile di nome 'Relaxed-Lenses' che puoi eseguire con `./Relaxed-Lenses` o cliccandolo due volte se per qualche disgrazia foste su Windows (scherzo ovviamene, non funziona per nente al momento su windows, anche se in teoria dovrebbe compilare ugualmente).

Feliz Navidad.
