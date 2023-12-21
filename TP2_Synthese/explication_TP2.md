TP2 Synthèse
-

L'objectif principal était de créer un client TFTP pour permettre l'échange de fichiers en utilisant le protocole TFTP (Trivial File Transfer Protocol).
C'est est un protocole de transfert de fichiers simple, utilisé pour le transfert de fichiers entre un client et un serveur sur un réseau. 
Conçu pour être léger et minimaliste, le TFTP fonctionne sur la couche de transport de données (généralement UDP) sans authentification ni sécurité intégrée.

Pour réaliser cela, j'ai dû me référer aux RFC, notamment RFC1350, RFC2347, RFC2348, RFC2349 et RFC7440 pour comprendre comment s'échanger des fichiers avec le serveur
TFTP.

On utilise deux fonctions gettftp et puttftp. En donnant en argument le host (nom du domaine ou adresse du serveur) et le nom du fichier à échanger.

Première étape : 
-
Avant de commencer à définir les fonctions gettftp et puttftp, il faut gérer les arguments données par l'utilisateur. Pour ceci on alloue de la mémoire avec malloc 
aux arguments argv[1] pour le nom de la commande (gettftp et puttftp), puis argv[2] l'adresse du serveur et enfin argv[3] pour le nom du fichier.

Deuxième étape : 
-
Réservation d'un socket : 
On a besoin de réserver un socket, on initialise une structure qui représente le point de communication entre le client TFTP
et le serveur TFTP distant. Ce socket est créé avec une famille d'adresses (IPv4, IPv6, etc.) et un type de communication (par exemple, SOCK_DGRAM
pour le protocole UDP utilisé par le TFTP).

La fonction gettftp : 
-
Cette fonction permet de télécharger un fichier depuis un serveur distant. Pour ceci il y a plusieurs étapes à respecter.
1) Envoyer une requête de lecture RRQ
2) Réception des paquets de données (DAT - Data Packets)
3) Gestion de la réception et construction des acquittements (ACK)

Explication : 
-
1) RRQ
   
    2 bytes     string    1 byte     string   1 byte
    ------------------------------------------------
   | Opcode |  Filename  |   0  |    Mode    |   0  |
    ------------------------------------------------

On envoie la requête sous la forme de paquet comme montré ci-dessus. Le Opcode est pour préciser le type de message, ici RRQ = 1.
On a besoin du nom du fichier "filename" et du mode, ici "octet". Pour envoyer cette requête on utilise la fonction sendto, où l'on précise 
le socket utilisé pour envoyer la requête, le message, et l'adresse du serveur.

2) Data packets
   
    2 bytes     2 bytes      n bytes
     ----------------------------------
    | Opcode |   Block #  |   Data     |
     ----------------------------------

Après la requête on recoit le paquet de data sous la forme ci-dessus. Pour cette partie il faut créer un fichier du même type que "filename"
puis écrire le data dans ce fichier. Mais comme on n'a pas pu le faire. On a simplement écrit sur la console ce qui était écrit dans ce fichier.
On l'a fait pour un fichier .txt, et donc il suffisait de copier et coller "data" sur la console. Le "block" précise l'ordre du data. En effet, dans le cas
où il y a plusieurs data il faut que l'on sache l'ordre dans lequel on doit le recevoir, on étiquette chaque paquet de data.

3) ACK

      2 bytes     2 bytes
      ---------------------
      | Opcode |   Block #  |
      ---------------------
   
Après réception du data, il faut prévenir le serveur qu'on a bien reçu les datas, on fait un accusé de réception. Comme précédement, le "Block" contient le
numéro du data, pour que le serveur sache quel data on a reçu.

La fonction puttftp : 
-
Cette fonction permet d'envoyer un fichier du local vers le serveur. Il y a donc plusieurs étapes à suivre.
1) Envoie de la requête de demande d'écrite WRQ
2) L'envoi du fichier que l'on veut envoyer
3) Réception de son acquittement du serveur pour la confirmation

On a à peine commencé cette partie, mais le principe est le même qu'expliqué pour le gettftp.







