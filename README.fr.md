<img width="4000" height="1482" alt="20260613_152410" src="https://github.com/user-attachments/assets/d5dda976-9baf-4767-b92b-becf465d58ba" />

<p align="center">
  <a href="README.md">
  <img src="https://flagcdn.com/w40/gb.png" width="40" height="28"><img src="https://img.shields.io/badge/English-Switch-blue?style=for-the-badge">
  </a>
  <img src="https://flagcdn.com/w40/fr.png" width="40" height="28"><img src="https://img.shields.io/badge/Français-Actuel-green?style=for-the-badge">
</p>

# Table des matières

* [Introduction](#Introduction)
* [Description rapide](#Description-rapide)
* [Micrologiciel](#Micrologiciel)
* [Architecture](#Architecture)
* [Caractéristiques](#Caractéristiques)
* [Images](#Images)
* [Composants](#Composants)
* [Instructions](#Instructions)
* [Schéma électrique](#Schéma-électrique)
* [PCB](#PCB)

# Introduction

Il s'agit d'un projet que j'ai réalisé pour le plaisir et afin de me lancer un défi durant ma deuxième année d'études collégiales en génie électrique. Comme je travaillais chez 3dkeycap (un fabricant de keycaps personnalisables à petite échelle), j'ai eu l'idée de concevoir un clavier mécanique séparé. Comme j'utilisais un clavier optique pleine grandeur et que mon père est programmeur, j'ai décidé d'en fabriquer un pour lui également. Il s'agit aussi de mon premier dépôt GitHub.

Comme je ne connaissais pas encore très bien les claviers séparés, le groupe de touches pour les pouces s'est retrouvé un peu inhabituel. Puisque ce clavier était destiné à un programmeur, il devait absolument posséder une rangée de touches de fonction. À ce moment-là, je savais déjà qu'il serait plus grand que des dispositions standards comme le Lily58 ou le Corne. Puisqu'il allait déjà être plus imposant, je me suis dit : « Pourquoi ne pas le rendre encore plus grand ? » J'ai donc fini par lui ajouter 80 touches.

Voici donc le projet terminé.

# Description rapide

Le MoCAM80 est un clavier mécanique séparé de 80 touches alimenté par des Raspberry Pi Pico 2. Il utilise un PCB réversible, ce qui signifie qu'un seul design de PCB est nécessaire pour les deux côtés du clavier.

# Micrologiciel

Le clavier utilise un micrologiciel personnalisé développé spécifiquement pour le MoCAM80.

La moitié gauche agit comme contrôleur principal tandis que la moitié droite fonctionne comme contrôleur secondaire connecté via un câble TRRS utilisant une communication UART. Des fichiers UF2 précompilés sont fournis dans le dépôt pour les deux moitiés.

# Architecture

* PCB gauche : contrôleur principal (USB HID)
* PCB droit : contrôleur secondaire de la matrice
* Communication : UART via TRRS
* Micrologiciel : C/C++ personnalisé

# Caractéristiques

* Un groupe de 6 touches pour les pouces
* Une rangée numérique
* Une rangée de touches de fonction
* Un boîtier imprimable en 3D
* Un PCB réversible (le même design est utilisé pour les deux côtés)

# Images

<img width="4000" height="2252" alt="20260613_152410" src="https://github.com/user-attachments/assets/0b5355a9-98e4-48b8-a1e2-0a019dc21814" />

<img width="4000" height="2252" alt="20260621_113730" src="https://github.com/user-attachments/assets/f69631d5-2efb-4f8f-959e-452af3b6b65c" />

<img width="4000" height="1786" alt="20260613_152818" src="https://github.com/user-attachments/assets/78d0a90b-0a48-40f8-94c4-0ce5ff59f23b" />

<img width="4000" height="2252" alt="20260621_113655" src="https://github.com/user-attachments/assets/59d9e749-bfc0-4316-8ee8-2541b8102c20" />

<img width="2252" height="2250" alt="20260613_152758" src="https://github.com/user-attachments/assets/f869b258-23d3-4cb1-96ae-86ac787764e4" />

<img width="2252" height="2334" alt="20260621_113712" src="https://github.com/user-attachments/assets/9c672538-4b03-4efc-b816-a55b47f40afc" />

<img width="2252" height="2250" alt="20260613_152807" src="https://github.com/user-attachments/assets/7e994947-3f52-4ee4-b6a3-fb6306ed325d" />

<img width="2252" height="2360" alt="20260621_113705" src="https://github.com/user-attachments/assets/e4b0c5f8-aa52-4256-ba11-5e75b806aef3" />

<img width="4000" height="2252" alt="20260613_152824" src="https://github.com/user-attachments/assets/0d93a70d-f706-4f48-88c9-4cb9090010f7" />

# Composants

## Assemblage du PCB

### Requis

| Quantité | Composant                           |
| -------- | ----------------------------------- |
| 80       | Interrupteurs Choc V2               |
| 80       | Keycaps MX à profil bas             |
| 2        | Raspberry Pi Pico 2                 |
| 2        | Connecteurs femelles TRRS (4 pôles) |
| 1        | Câble TRRS (4 pôles)                |
| 1        | Câble de données Micro-USB          |

### Optionnel

| Quantité | Composant                                                               |
| -------- | ----------------------------------------------------------------------- |
| 12       | Résistances de 10 kΩ (les résistances de tirage internes sont activées) |

## Boîtier

| Matériau | Quantité |
| -------- | -------- |
| Filament | 300 g    |

# Instructions

1. Une fois les deux PCB en votre possession, soudez un Raspberry Pi Pico 2 sur chacun d'eux.

2. Soudez un connecteur femelle TRRS sur chaque PCB. (Manipulez-les avec précaution, car une force excessive lors du branchement ou du débranchement du câble pourrait arracher les pastilles du PCB.)

3. Soudez 40 interrupteurs sur un PCB et 40 interrupteurs sur l'autre PCB.

4. OPTIONNEL : Soudez 6 résistances de 10 kΩ sur chaque PCB.

5. Sur le PCB gauche (celui dont les interrupteurs se trouvent du même côté que le Pico 2 et le connecteur TRRS), coupez les deux pistes supérieures illustrées ci-dessous, situées près du coin inférieur droit du Pico 2.

<p align="center">
<img width="225" height="400" alt="20260613_170107" src="https://github.com/user-attachments/assets/b51ae2ce-d883-488a-8660-12851e16e584" />
</p>

6. Sur le PCB droit (celui dont les interrupteurs se trouvent du côté opposé au Pico 2 et au connecteur TRRS), coupez les deux pistes inférieures illustrées ci-dessous, situées près du coin inférieur droit du Pico 2.

<p align="center">
<img width="225" height="400" alt="20260613_170107" src="https://github.com/user-attachments/assets/bcee4b3d-b296-48c0-85e2-dff882e02128" />
</p>

7. Téléversez le [fichier UF2 pour la carte principale](Software/Compiled%20Files/keyboard_Main.uf2) sur le PCB gauche.

8. Téléversez le [fichier UF2 pour la carte secondaire](Software/Compiled%20Files/keyboard_Secondary.uf2) sur le PCB droit.

9. Vérifiez que tout fonctionne correctement.

10. Imprimez le boîtier et assemblez le produit final.

# Schéma électrique

<img width="1067" height="824" alt="image" src="https://github.com/user-attachments/assets/b36e67d7-3369-43a0-9a87-0c093f392f98" />

# PCB

## Face supérieure

<img width="975" height="851" alt="image" src="https://github.com/user-attachments/assets/36280b8b-0c83-4277-8057-dafde2d735b7" />

## Face inférieure

<img width="974" height="853" alt="image" src="https://github.com/user-attachments/assets/fe5c67c6-a313-4545-89d1-a70f8e09f524" />
