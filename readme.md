# Configuration de l'environnement de dev
Arduino IDE + http://www.embedded-computing.com/embedded-computing-design/getting-started-with-the-wemos-d1-mini-esp8266-dev-board
Et là il y a pein d'exemples fournis avec l'IDE

# OLED
https://automatedhome.party/2017/04/17/connect-an-inexpensive-oled-display-to-a-esp8266wemos-d1-mini/
Hyper important !! pour que ça marche il faut positionner le OLED_RESET à 0 (zéro)
En plus de modifier le .h : https://forum.arduino.cc/index.php?topic=364400.0 (erreur Height incorrect, please fix Adafruit_SSD1306.h!)

# Serveur Web
https://techtutorialsx.com/2016/10/22/esp8266-webserver-getting-query-parameters/
Voir les exemples fournis avec l'IDE

# Wifi
Voir les exemples fournis avec l'IDE


 # Programmer l'ESP-01
 Avec le module USB to ESP8266 il faut conneter GPIO0 avec GND. Une pince croco et hop !
