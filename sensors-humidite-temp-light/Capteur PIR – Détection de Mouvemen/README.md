# PIR Sensor – ESP32 Project

## Description
Ce code lit le capteur **PIR** pour détecter le mouvement et allume une LED lorsque le mouvement est détecté.

## Matériel
- ESP32
- Capteur PIR
- LED + résistance

## Connexion des composants
| PIR | ESP32 |
|-----|-------|
| VCC | 5V |
| GND | GND |
| OUT | GPIO 5 |

| LED | ESP32 |
|-----|-------|
| Anode | GPIO 2 |
| Cathode | GND (avec résistance) |

## Fonctionnement
1. ESP32 lit l'état du PIR  
2. Si mouvement détecté → LED ON, sinon LED OFF  
3. Affiche le résultat sur le moniteur série
