# Hi, welcome!
This is my attempt at putting a computer in my beloved car. For now it will be primarily used for EQing all of my speakers, but later I plan to replace the current android radio in the car with a touchscreen display that will be connected to the computer, and that will be controlled with a custom UI (probably made in Qt).

### PC Specs:
- Motherboard: Gigabyte Z68AP-D3
- CPU: Intel Core i3-3240
- GPU: Integrated Intel HD Graphics 2500
- RAM: 1x4GB
- Sound Card: Creative Sound Blaster Audigy RX

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