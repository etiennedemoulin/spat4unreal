# UE.SPAT
v.0.3 - 26.08.2020

Max 8.1.5
______________________

# General configuration

## Unreal
add unreal source code into your project. Use BPSpatSound to place a new audio file, BPSpatRoom to place a new room.

## Max
Soundfiles must be placed into ue.spat/audio

Start MAIN.maxpat, don't forget to add ue.spat folder in Max File Preferences.
Select configuration and click the button.
Configure audio in Max : buffersize and IOVS tested with 256.

# Loudspeaker configuration
[Ambidecodertoolbox](https://bitbucket.org/ambidecodertoolbox/adt/src/master/) is used to generate speaker configuration. Please install it.

Use ue_speaker.m as a template to configure your speaker, then compile using [GNU Octave](https://www.gnu.org/software/octave/). You need 5 channels minimum.

The output file is a .dsp file, please rename it ue.speaker.dsp
## Edit ue.speaker.dsp 
Please consider the following variables : 

	`decoder_type` : 1 is full band decoder, 2 is for using with a separate subwoofer.

Compile with Faust : `faust2max6 ue.speaker.dsp`

Put ue.speaker.mxo in ue.spat/externals

# Ambisonic conventionning

component ordering : ACN

normalisation : N3D


