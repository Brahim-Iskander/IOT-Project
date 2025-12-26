# LDR Sensor – ESP32 Project

## Description
Ce code lit la valeur d'un **capteur LDR** (luminosité) et contrôle une LED selon la lumière ambiante.

## Matériel
- ESP32
- LDR + résistance (pont diviseur)
- LED + résistance

## Connexion des composants
| LDR | ESP32 |
|-----|-------|
| VCC | 5V |
| GND | GND |
| Signal | GPIO 12 |

| LED | ESP32 |
|-----|-------|
| Anode | GPIO 2 |
| Cathode | GND (avec résistance) |

## Fonctionnement
1. Lire la valeur du LDR  
2. Si luminosité faible → LED ON  
3. Affiche la valeur sur le moniteur série
