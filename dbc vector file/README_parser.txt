
FONCTIONS DEVELOPPEES:
**************************
1. Extraction d'un fichier dbc 

2. Enregistrement des trames et signaux de trames, ainsi que tous leurs param�tres dans lists

3. Lecture d'un signal � partir d'une trame, et conversion en sa valeur physique

4. Conversion de la valeur physique d'un signal en valeur Hexadecimale.

5. construction de la trame � partie des signaux.

NOTE: 
- le parser prend en compte les "byte orders" MOTOROLA et INTEL en lecture et �criture, ainsi que les valeurs sign�es.
- L'�criture dans un signal n'efface pas les autres octets de la trame fournie par le main.


POUR TESTER LE PROGRAMME: - modifier les varaiables dans le main (signal, frame_ID et/ou physical value) lancer le parser.exe depuis une fenetre de commande.




RESTE A FAIRE:
**************
1 Faire un syst�me permettant de sauvegarder le contenu pr�c�dent  des trames CAN afin de ne pas  perdre les donn�es lors de r��criture dans un signal.

2 Faire la liaison Emission/R�ception CAN <==> parser

3 Faire une fonction permettant de convertir tous les signaux d'une trame en valeur physique en m�me temps.

4 Adapter � ARDUINO et lecture SD

5 Optimisation du code et types de variables
... 




