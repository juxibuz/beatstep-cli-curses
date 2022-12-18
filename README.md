Use sysex to control BeatStep

## usage

```
Options:
  -h,--help                   Print this help message and exit

Subcommands:
  list                        List available MIDI devices
  load                        Load a .beatstep preset file on device
  save                        Save a .beatstep preset file from device
  color                       Change color of an LED
  fw                          Get the firmware version on the device
  get                         Get a param-value
  set                         Set a param-value
  curses                      Launch curses interface (device defaults to 2)
```

### examples

```
# get the firmware revision of your device
beatstep fw

# set first pad to blue
beatstep color 0 blue

# set first pad to red
beatstep color 0 red

# set first pad to pink
beatstep color 0 pink

# turn it back off
beatstep color 0 off

# load a preset from a file
beatstep load mine.beatstep

# save a preset to a file
beatstep save mine.beatstep

# get the setting for 0:82
beatstep get 0 82

# set the setting for 0:82 to 0
beatstep set 0 82 0

# curses interface
t - set pad to toggle
T - set pad to toggle and move to next pad
g - set pad to gate
G - set pad to gate and move to next pad
, - decrease midi note
. - increase midi note
left and right arrow moves between pads
```


## dev

```
git clone --recursive https://github.com/konsumer/beatstep-cli.git
cd beatstep-cli

cmake -B rtmidi/build rtmidi
cmake --build rtmidi/build

cmake -B build
cmake --build build

./build/beatstep --help
```
