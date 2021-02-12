# power-safe

Projet d'alarme a detection humaine utilisant :
+ ESP8266
+ capteur PIR Arduino
+ Speaker
+ Keypad

Scenario de fonctionnement : 
+ Activer l'alarme en saisissant un code a 4 chiffre sur le Keypad
+ l'Alarme est maintenant activer
+ Si un humain rentre dans le champs de detection le Speaker retenti
+ Pour desactiver l'alarme il faut rentrer le bon mots de passe 
+ Si un mauvais mots de passe est rempli l'alarme retenti
+ Il est possible de changer le mot de passe lorsque l'alarme est desactiver

Liste des requetes possible a faire sur le serveur de l'esp : 
+ GET /getAlarm  => Retourne true si l'alarme est activer false sinon
+ GET /desactivateAlarm  => parametre : password    => desactive l'alarme avec le mot de passe passer en parametre
+ GET /new => parametre : password     => change le mots de passe en cours si l'alarme est activer
