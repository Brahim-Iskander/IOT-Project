# DHT11 Sensor – ESP32 Project

## Description
Ce code lit la **température et l’humidité** avec le capteur DHT11 et affiche les données sur le moniteur série.

## Matériel
- ESP32
- DHT11
- Câbles de connexion

## Connexion des composants
| DHT11 | ESP32 |
|-------|-------|
| VCC | 5V |
| DATA | GPIO 15 |
| GND | GND |

##Fonctionnement – DHT11 + ThingSpeak (from zero)

1-Créer un compte ThingSpeak
    Aller sur https://thingspeak.com
    Cliquer sur Sign Up et créer un compte gratuit

2-Créer un channel pour le DHT11
    Une fois connecté, cliquer sur Channels → My Channels → New Channel
    Nommer le channel, par exemple : ESP32 Temperature & Humidity
    Activer Field 1 pour la Température et Field 2 pour l’Humidité
    Cliquer sur Save Channel

3-Récupérer la Write API Key
    Dans le channel, cliquer sur API Keys
    Copier la clé Write API Key (exemple : QFFVQ0KN51FUHFP7)
4-Modifier le code ESP32
    Dans ton code, remplacer String apiKey = "TON_WRITE_API_KEY"; par ta clé API copiée

    Exemple :
    String apiKey = "QFFVQ0KN51FUHFP7";

5-Connecter le DHT11 à l’ESP32
    VCC → 5V
    DATA → GPIO 15
    GND → GND

6-Téléverser le code sur l’ESP32
    Brancher l’ESP32 à l’ordinateur
    Ouvrir l’IDE Arduino
    Sélectionner la carte ESP32 correspondante et le port
    Cliquer sur Téléverser

7-Vérifier la communication
    Les données sont envoyées toutes les 15 secondes sur ThingSpeak
8-Voir les données sur ThingSpeak
    Aller sur le channel créé
    Les valeurs de Field1 (Température) et Field2 (Humidité) sont mises à jour en temps réel
    Tu peux créer des graphes ou exporter les données

9-Utiliser les données dans un autre projet

    Récupérer les données via HTTP GET en utilisant la Read API Key de ton channel
    Exemple d’URL pour lire les données JSON :
        https://api.thingspeak.com/channels/CHANNEL_ID/fields/1.json?api_key=READ_API_KEY&results=10

    Avec cette URL, tu peux :
        Intégrer les données dans une application web
        Créer des alertes ou tableaux de bord IoT
        Utiliser les données dans Arduino/ESP32 ou Raspberry Pi pour d’autres projets
        Lire les champs Field1 (Température) et Field2 (Humidité) selon tes besoins