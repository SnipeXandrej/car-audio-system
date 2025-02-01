# Hi, welcome!
This is my attempt at putting a computer in my beloved car. For now it will be primarily used for EQing all of my speakers, but later I plan to replace the current android radio in the car with a touchscreen display that will be connected to the computer, and that will be controlled with a custom UI (probably made in Qt).

### Things I would like to add/implement:
- [ ] Battery voltage monitoring
- [ ] Schematics of the whole ordeal
- [ ] Custom UI
  - - [ ] Show current song that's being played either through Bluetooth or through the computer itself
  - - [ ] Bluetooth: play/pause, next, previous buttons
- [ ] Remove included Android Radio and replace it with a touchscreen connected to the computer itself
- [ ] Physical Buttons for volume up/down, play next/previous song, etc.
- [ ] AUX Input

### Things already implemented:
- [x] Communication between "EC" and the OS

# Commands and their actions between EC and Userspace

### EC
| Received message/(Action)     | Output message/(Action)                 |
|-------------------------------|-----------------------------------------|
| ARE_YOU_ALIVE                 | YES_I_AM_ALIVE                          |
| GET_VOLTAGE_REMOTE            | VOLTAGE_REMOTE=%d                       |
| PC_POWER_OFF                  | PC_POWER_OFF (Powers the computer off)  |

### Userspace
| Received message/(Action)   | Output message/(Action)                                       |
|-----------------------------|---------------------------------------------------------------|
| YES_I_AM_ALIVE              | (Allows for all other commands than this command to function) |
| PC_POWER_OFF                | (Powers the computer off)                                     |

### To be implemented:
#### EC
| Received message/(Action) | Output message/(Action)                     |
|---------------------------|---------------------------------------------|
| AMPx_POWER_ON             | AMPx_POWERED_ON (Powers on amplifier n.x)   |
| AMPx_POWER_OFF            | AMPx_POWERED_OFF (Powers off amplifier n.x) |
| GET_VOLTAGE_BATTERY       | VOLTAGE_BATTERY=%f                          |

# Hardware and software and stuff...
### PC Specs:
- Motherboard: Gigabyte Z68AP-D3
- CPU: Intel Core i3-3240
- GPU: Integrated Intel HD Graphics 2500
- RAM: 1x4GB
- Sound Card: Creative Sound Blaster Audigy RX
- Bluetooth: USB Adapter ORICO BTA-508 with Bluetooth 5.0

### Software:
- Operating System: [Arch Linux](https://archlinux.org/)
- Compositor: [labwc](https://github.com/labwc/labwc)
- Audio Server: [PipeWire](https://pipewire.org/)
- Audio Plugins: [LSP](https://lsp-plug.in/)
- Audio Plugin Host: [Carla](https://kx.studio/Applications:Carla)

### ec-userspace / ec-atmega8
These folders contain code that allows communication between linux and the Atmega8 based ""EC"" through Serial

- EC as in ""Embedded Controller""
- Primary purpose of this EC is to control the power coming into the whole PC, and acts as a protection, so the computer can't drain the battery if it for some reason won't turn off...

### linux
This directory contains things like workarounds, custom packages, config files, etc. used in linux (excluding `ec-userspace`)
