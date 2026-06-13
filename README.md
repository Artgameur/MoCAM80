# Table of Contents

* [Introduction](#Introduction)
* [Firmware](#Firmware)
* [Features](#Features)
* [Images](#Images)
* [Components](#Components)
* [Instructions](#Instructions)

# Introduction

This is a project that I made for fun and to challenge myself during my second year of college as an electrical engineering student. Since I was working at 3dkeycap (a small-scale customizable keycap manufacturer), I thought about making a mechanical split keyboard. Since I was using a standard-size optical keyboard and my father is a programmer, I thought of making one for him too. This also happens to be my first GitHub repository.

Since I didn't know much about split keyboards, the thumb cluster ended up being a little unusual. Because I was making a keyboard for a programmer, it had to have a function row. At that point, I knew that the keyboard was going to be larger than standard layouts such as the Lily58 or Corne. Since it was already going to be that large, I thought, "Why not make it bigger?" So I ended up adding 80 keys.

So here is my finished project.

# Firmware

The keyboard uses custom firmware written specifically for the MoCAM80.

The left half acts as the primary controller while the right half operates as a secondary controller connected through TRRS via UART. Precompiled UF2 files are provided in the repository for both halves.

# Features

* A 6-key thumb cluster
* A number row
* A function row
* A 3D-printable case
* A reversible PCB (the same design is used for both sides)

# Images

<img width="4000" height="2252" alt="20260613_152410" src="https://github.com/user-attachments/assets/0b5355a9-98e4-48b8-a1e2-0a019dc21814" />

<img width="4000" height="1786" alt="20260613_152818" src="https://github.com/user-attachments/assets/78d0a90b-0a48-40f8-94c4-0ce5ff59f23b" />

<img width="2252" height="2250" alt="20260613_152758" src="https://github.com/user-attachments/assets/f869b258-23d3-4cb1-96ae-86ac787764e4" />

<img width="2252" height="2250" alt="20260613_152807" src="https://github.com/user-attachments/assets/7e994947-3f52-4ee4-b6a3-fb6306ed325d" />

<img width="4000" height="2252" alt="20260613_152824" src="https://github.com/user-attachments/assets/0d93a70d-f706-4f48-88c9-4cb9090010f7" />

# Components

## PCB Assembly

### Necessary
| Quantity | Component |
|----------|-----------|
| 80 | Choc V2 switches |
| 80 | MX low-profile keycaps |
| 2 | Raspberry Pi Pico 2 |
| 2 | TRRS (4-pole) female connectors |
| 1 | TRRS (4-pole) cable |
| 1 | Micro-USB data cable |

### Optional
| Quantity | Component |
|----------|-----------|
| 12 | 10 kΩ resistors (internal pull-ups are enabled) |

## Case
| Material | Component |
|----------|-----------|
| filament | 300g |

# Instructions

1. Once you have 2 PCBs, solder 1 Raspberry Pi Pico 2 onto each PCB.

2. Solder 1 female TRRS connector onto each PCB. (Be careful when handling them, as excessive force while plugging or unplugging the cable may lift pads from the PCB.)

3. Solder 40 switches onto one PCB and 40 switches onto the other PCB.

4. OPTIONAL: Solder 6 10 kΩ resistors onto each PCB.

5. On the left-hand PCB (the one with the switches on the same side as the Pico 2 and the TRRS connector), cut the upper two traces shown below, located near the bottom-right corner of the Pico 2.

<p align="center">
<img width="225" height="400" alt="20260613_170107" src="https://github.com/user-attachments/assets/b51ae2ce-d883-488a-8660-12851e16e584" />
</p>

6. On the right-hand PCB (the one with the switches on the opposite side of the Pico 2 and the TRRS connector), cut the lower two traces shown below, located near the bottom-right corner of the Pico 2.

<p align="center">
<img width="225" height="400" alt="20260613_170107" src="https://github.com/user-attachments/assets/bcee4b3d-b296-48c0-85e2-dff882e02128" />
</p>

7. Upload the [UF2 file for the main board](Software/Compiled%20Files/keyboard_Main.uf2) to the left-hand PCB.

8. Upload the [UF2 file for the secondary board](Software/Compiled%20Files/keyboard_Secondary.uf2) to the right-hand PCB.

9. Confirm that everything works correctly.

10. Print the case and assemble the final product.
