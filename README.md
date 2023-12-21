# TP1_Synthese

Pendant ce travail pratique, nous avons appris à concevoir et à mettre en place un petit interpréteur de commandes que nous avons appelé "enseash". 
L'objectif principal était de créer un shell minimal capable de lancer des programmes tout en affichant les codes de retour et les temps d'exécution. Voici les sujets abordés et les compétences acquises :

Création d'un shell de base : Nous avons mis en place un interpréteur de commandes rudimentaire avec les fonctionnalités essentielles comme un message de bienvenue, 
une invite de commande et la gestion des entrées utilisateur.

Boucle de lecture-évaluation-affichage :
Nous avons implémenté une boucle qui lit la commande saisie, l'exécute, puis repasse à l'invite en attendant la prochaine instruction.
C'est dans cette étape que nous apprennons plus de l'utilisation de fork pour les processus enfants et parents. Dans le processus enfant, il y a la gestion de l'éxecution de la commande avec la fonction execlp.
Il y avait aussi quelques détails comme le fait qu'il fallait enlever le "\n" de la saisie de commande par l'utilisateur pour mettre un "\0" sans quoi la fonction execlp ne comprend pas la commande.

Gestion de la sortie du shell :
Nous avons ajouté la possibilité de quitter proprement le shell en utilisant la commande "exit". Rien de plus compliqué, on regarde si la saisie est "exit" et on écrit un message
sur le shell "bye bye"

Affichage du code de retour :
Le shell affiche désormais le résultat (ou signal) de la commande précédente dans l'invite, fournissant des informations sur le statut de la dernière commande exécutée.
Pour vérifier le signal de retour nous avons du utiliser l'instruction kill -s SIGTERM pid où il fallait remplacer pid par la valeur réelle. Pour cela nous devions faire un getpid() puis mettre un sleep pour avoir
le temps de le recopier dans kill -s SIGTERM pid. (SIGTERM n'est qu'un exemple de signal)

Mesure du temps d'exécution : 
Nous avons intégré la mesure du temps pris par les commandes en utilisant l'appel système clock_gettime. Cela permet d'afficher le délai pour chaque commande exécutée dans l'invite.
Il fallait utiliser une structure de la bibliothèque time.h. Une deux variables stockent le temps écoulé depuis sa création et il suffisait de faire la différence de ces deux variables pour connaitre le temps écoulé
entre le moment de la saisi de la commande et son exécution.

Exécution de commandes avec arguments :
Nous avons étendu les fonctionnalités du shell pour prendre en charge l'exécution de commandes complexes avec des paramètres.
Le principe reste le même mais il faut séparer les différents arguments de la saisie par l'utilisateur. On utilise maintenant la fonction execvp et non execlp, car on peut maintenant avoir plusieurs arguments.
Le reste du code reste le même.
