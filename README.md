# Temperament Generator

## by Lewis Wolf

Written in Max MSP v.8+

Dependencies: [bach library v0.8.0b+](https://www.bachproject.net/dl/)

The _Temperament Generator_ was developed for real time implementations of microtonal tunings, for use in both compositional and performative settings. The patch works both as a standalone application, running in Max MSP, as well as in Max for Live, with only a small amount of user patching necessary.

A demonstration video for this project can be found [here](https://youtu.be/C4txE2hQ53w).

## Basic Use Guide

The _Temperament Generator_ takes in raw MIDI and outputs MIDI with precise pitch bend values relative to the user defined mapping and temperament. The output can be formatted either as standard 14-bit pitch bend values, or as three CC messages primarily for use with Kontakt (for more information on using this patch with Kontakt, please see [here](https://github.com/lewiswolf/Polyphonic-Microtuning-in-Kontakt-with-Max-MSP)). Both outputs make use of all 16 MIDI channels to allow for notes to be independently tuned. By toggling _Fluid_ to on, the intonation of held notes can be changed immediately, however this is achieved by repeatedly resending the held MIDI message with the updated pitch bend values, and may cause unwanted interferance.

A user can create up to 10 simultaneous temperaments and map them onto a MIDI device. For each temperament, there are three basic modes available.

-   **EQ**  
    for creating equally divided temperaments.
-   **Scala**  
    for importing .scl files and typing out your own in the console. The syntax for this is documented [here](http://www.huygens-fokker.org/scala/scl_format.html).
-   **Series**  
    for parsing sequential equations, such as those that generate the harmonic series. The syntax for this mode is documented in the `bach.expr` helpfile contained within the _bach library_. `k` is used to represent the sequence of integers `1 <= k <= n` where `n` is the number of _Notes_ detailed in the table below.

When creating each temperament, information is entered into a table like the one shown here.

| Type | Reference | Top | Notes | Multiplier |
| :--: | :-------: | :-: | :---: | :--------: |
|  EQ  |    440    | 880 |  12   |     1      |

This input produces 12TET, with 440hz as a reference pitch and 880hz as a repeat point (octave or psuedo-octave). With _Multiplier_ set to 1, the temperament will cover the entire 128 MIDI note range. When set to 0 this temperament bypasses the _Multipler_, and all that will be output is 12 pitches between 440hz and 830.61...hz (A4 - G#5). _Reference_ and _Top_ frequencies can be input as floating points or integers, and _Top_ also accepts ratio values (i.e 2/1).

When working in _Scala_, the psuedo-octave is supplied by the syntax. Changing the _Top_ frequency instead distorts the temperament, whilst maintaing relative interval structure. This is similar to working in _Series_, such that when using the Pythagorus preset (`log(k)/log(2)`) to create the harmonic series, this can also be distorted by changing the value for _Top_, again whilst maintaining relative interval structure.

The Mapping can be calibrated using the _Mapping Settings_ which offers two methods for mapping structures depnding on whether or not they are multiplied.

-   **General Settings**  
    The global output range can be specified here, however the default values correlate to MIDI note 0 and 127.

-   **Without Multiplier (Group 0)**  
     When using this setting, Temperaments are ordered according to the order in which they were input into the table. The reference MIDI note is used to determine the starting note for all Group 0 Temperaments. Group 0 temperaments can either overdub notes generated by Group 1, or be inserted inbetween them.

-   **With Multiplier (Group 1)**  
    Temperaments are merged such that all pitches fall in order and cover the entire 128 MIDI note range. The reference MIDI note corresponds to the reference pitch of a given structure, such that, if the reference pitch is 440hz and the reference note is D5, playing D5 will produce 440hz and all pitches surrounding it will be ordered accordingly.

## Using the Temperament Generator with your own Max MSP and Max for Live patches

When using the _Temperament Generator_ with Max for Live, it is important to note that Live does **not** support the use of mutliple MIDI channels. There have been many workarounds devised to get around this, however to simplify the matter, the _Temperament Generator_ should be used within a Max Instrument, and not as a seperate Max MIDI device. A typical setup is shown in the 'demo amxd' folder.

To help speed up the process of using the Temperament Generator with your own Max MSP or Max for Live patches, a bpatcher and an abstraction have also been supplied.

-   **midibae**  
    This bpatcher is designed to work sympathetically with a `poly~`. The left outlet outputs MIDI notes (pitch and velocity), as well as `target` & `legacynotemode` messages for the `poly~`. These are controlled using _Ignore MIDI Channel/Target MIDI Channel_. _Target MIDI Channel_ targets `poly~` voices relative to the incoming MIDI channel, and is used to make sure pitch bend values are always paired with the correct MIDI note. When using _Target MIDI Channel_, it is recommended that your `poly~` have 16 voices - one for each MIDI channel. The middle outlet outputs 14bit MIDI pitch bend values. The right outlet outputs raw MIDI.

-   **mtof&c**  
    This works similarly to the `mtof` object in Max, but takes pitch bend values in the right inlet. Outputs frequency from the left outlet and cents from the right outlet. 0 cents = 440hz.

_midibae_ and the _Temperament Generator_ both accept raw MIDI in the left inlet, as well as two messages.

1. `M4L 1` and `M4L 0` which is necessary to tell the patcher whether or not it is running in Max for Live. In both patchers this sets the MIDI input to come from the leftmost inlet rather than an internal `midiin`.

2. `allnotesoff` which should be used to prevent stuck notes.

_midibae_ also accepts the messages `mc 1` and `mc 0` which toggle _Target MIDI Channel_. The right inlet is used for cosmetic purposes, and can show or hide various UI elements.

-   `target`
-   `default`
-   `panic`

The right inlet of the _Temperament Generator_ is reserved for use with `pcontrol` and other messages to `thispatcher`.

## Automation & Presets

When in Live, the _Reference_, _Top_ & _Notes_ settings can be automated via the automation lanes.

-   Ref \- _Reference_
-   Top \- _Top_
-   Div \- _Notes_

Automation can also be controlled using several global sends from within Max.

-   `s autoPRE` \- to select presets
-   `s autoREF` \- _Reference_
-   `s autoTOP` \- _Top_
-   `s autoDIV` \- _Notes_

It should be noted that automation functionality is limited, and both the sends and automation lanes will only effect the individual Temperament last changed/selected. For best results, reserve automation for use with only one temperament or switching between presets.

Presets are global, and are stored in the parent folder. Updating presets whilst in Live will automatically update all instances of the patcher.

## File I/O

The _Temperament Generator_ is capable of loading scala tuning files (.scl) and can also export scala tuning files, Anamark tuning files (.tun) and spreadsheets (.csv). The exported scala file targets the currently selected temperament, transposed relative to 0 cents. The exported spreadsheet contains the complete mapping of the 128 MIDI note range. Any unused MIDI notes will not appear in the spreadsheet.
