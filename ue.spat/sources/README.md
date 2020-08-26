##Compilation instructions

##ue.source
For MacOS Max external :
faust2max6 -nopost ue.source.dsp
For Win64 Max external : 
faust2w64max6 ue.source.dsp


##circle
For MacOS Max external : 
? ask to Yann how to compile mxo in command line.
For Win64 Max external : 
? ..


##ue.rotator
For MacOS Max external : 
faust2max6 -nopost -t 0 ue.rotator.dsp
For Win64 Max external : 
faust2w64max6 -t 0 ue.rotator.dsp

##ue.binaural.decoder
For MacOS Max external : 
faust2max6 -nopost -t 0 ue.binaural.decoder.dsp
For Win64 Max external : 
faust2w64max6 -t 0 ue.binaural.decoder.dsp

##ue.loudspeaker.decoder
Create DSP Faust file :
copy st_etienne_9.m file in ambidecodertoolbox folder
open terminal in ambidecodertoolbox folder
> octave
> adt_initialize
> st_etienne_9

open terminal in ambidecodertoolbox/decoders
rename old.dsp file in ue.loudspeaker.decoder.dsp.
edit following parameters:
	- decoder_type = 1 // CONFIG W/OUT SUBWOOFER.
	- nfc_input = ?
	- output_gain_muting = 1


For MacOS Max external :
faust2max6 -nopost -t 0 ue.loudspeaker.decoder.dsp
For Win64 Max external :
faust2w64max6 -t 0 ue.loudspeaker.decoder.dsp