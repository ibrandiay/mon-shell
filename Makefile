

# le makefile de mon projet . Tres simple car y'a qu'un  fichier 

CC?= gcc
my_shell.exe:my_shell.o 
	@gcc my_shell.o -o  my_shell -lreadline
shell1.o:my_shell.c 
	@gcc -c shell1.c 
