#        Creation de shell 
Dans ce projet , il a été question de développer un Shell . Il est divisée en deux partie
La première partie a été achever avec succès . On a ainsi parvenu a implémente tout
les fonctionnalités demander de A1 à A7.
Pour la deuxième partie du travaille demander on n’a pas arrivé au bout mais certain
fonctionnalité on été traiter tel que : B8 , B9 ,B12 , B16, B18 , B19.  
La réalisation de l’historique des commande ainsi que le support de CTRL + R est
rendu possible par le bibliothèque readline fourni par GNU .
Nous l’avons donc télécharger et installer avec :
sudo apt-get install libreadline-dev
Et notre shell avec le peu de fonctionnalité qu’il possède , est très stable . Par
exemple pour installer readline j’ai utilisé mon shell et non le bash. On peut y lancer
GDB , lister les processus avec ps ...
#               COMPILATION
un petit makefile permet de compiler my_shell car tout le code est inscrit dans un seul
fichier comme convenue avec le prof. On peut aussi utiliser la ligne de commande
suivant :
 gcc my_shell.c -o my_shell -lreadline
On signale qu’en même que sans l’option -lreadline , une belle erreur de non
reconnaissance de fichier s’affiche . On a vraiment énormément galéré sur ça.
