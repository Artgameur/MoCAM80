# Table of Contents
- [Introduction](#Introduction)
- [Features](#Ieatures)
- [Images](#Images)
- [Components](#Components)
- [Instructions](#Instructions)

# Introduction

This is a projet that I made for fun and to challenge myself during my second year of college as an eleectrical engineering student. Since I was working at 3dkeycap (a small scale customizable keycap manufacturer), I thought of making a mechanical split keyboard since I had a standard size optical keyboard and, since my father is a programmer, I thought of making him one too. This also happens to be my first github repository. Since I don't know that much about split keyboards, the thumb cluster ended up being a little weird and since I was making a keyboard for a programmer, it had to have the function row. At that point, i knew that the keyboard was going to be bigger then the standard sizes like the Lily58 or Corne layout. Since it was going to be that big, I thought "why not make it bigger?" so I ended up putting 80 keys. So here is my finished project.


# Feature
- A 6 button thumb cluster
- The number row
- The function row
- A 3D printable case
- A reversible PCB (the same desing is used for both sides)

# Images

<img width="4000" height="2252" alt="20260613_152410" src="https://github.com/user-attachments/assets/0b5355a9-98e4-48b8-a1e2-0a019dc21814" />

<img width="4000" height="1786" alt="20260613_152818" src="https://github.com/user-attachments/assets/78d0a90b-0a48-40f8-94c4-0ce5ff59f23b" />

<img width="2252" height="2250" alt="20260613_152758" src="https://github.com/user-attachments/assets/f869b258-23d3-4cb1-96ae-86ac787764e4" />

<img width="2252" height="2250" alt="20260613_152807" src="https://github.com/user-attachments/assets/7e994947-3f52-4ee4-b6a3-fb6306ed325d" />

<img width="4000" height="2252" alt="20260613_152824" src="https://github.com/user-attachments/assets/0d93a70d-f706-4f48-88c9-4cb9090010f7" />

# Components

## PCB Assembly
### Necessary
- 80	:	Choc v2 switches
- 80	:	MX low profile keycaps
- 2		:	raspberry pi pico 2
- 2		:	TRRS (jack 4 pad) female connectors
- 1		:	TRRS (jack 4 pad) wire
- 1		:	micro-usb data cable

### Optional
- 12	:	10k ohms resistor (internal pull ups are active)

## Case
- 300g of filament


# Instructions

1) Once you have 2 PCBs, solder 1 raspberry pi pico 2 on each pcb.
2) Solder 1 female TRRS conncetor on each PCB. (Be carful when handling, as they can lift form the pcb with some pads if plugged to forcefully)  
3) Solder 40 switches on one side of the PCB and 40 on the other side of the other PCB
4) OPTIONAL Solder 6 10k ohms resistors on each PCB.
5) On the left hand PCB (the one with the switches on the same side as the pico 2 and the TRRS connector), next to the bottom right of the pico 2, cut the upper two traces shown below

<p align="center">
<img width="225" height="400" alt="20260613_170107" src="https://github.com/user-attachments/assets/b51ae2ce-d883-488a-8660-12851e16e584" />
</p>

6) On the right hand PCB (the one with the switches on the opposite side as the pico 2 and the TRRS connector), next to the bottom right of the pico 2, cut the lower two traces shown below

<p align="center">
<img width="225" height="400" alt="20260613_170107" src="https://github.com/user-attachments/assets/bcee4b3d-b296-48c0-85e2-dff882e02128" />
</p>

7) On the left hand PCB upload the [uf2 file for the main board](Software/Compiled%20Files/keyboard_Main.uf2).
8) On the right hand PCB upload the [uf2 file for the secondary board](Software/Compiled%20Files/keyboard_Secondary.uf2).
9) Confirm it works.
10) Print the case and assemble the final product.
