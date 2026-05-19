/*
Firmware for a gba-to-midi-cable with integrated sequencer/synthesizer
(C) 2011 Jeroen Domburg (jeroen AT spritesmods.com)

This program is free software: you can redistribute it and/or modify
t under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
	    
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
			    
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


//Definition of all the MIDI control channels, plus some knobs you can
//only tweak at the UI.

#include "ccs.h"
#include "sound.h"

/*
#define SNDFX_DUTYCYCLE 1
#define SNDFX_SWEEP_DUTYCYCLE 2
#define SNDFX_NOISE_BITS 3
#define SNDFX_SAMPLE_SETSAMP 4
#define SNDFX_PITCH 5
#define SNDFX_SAMPLE_PITCH 6
#define SNDFX_NOISE_PITCH 7
#define SNDFX_SWEEP_PITCH 8
#define SNDFX_SWEEP_VAL 9
#define SNDFX_SWEEP_SPEED 10
#define SNDFX_SWEEP_SNDLEN 11
#define SNDFX_SWEEP_ENVELOPE 12
#define SNDFX_DSNDSQUARE 13
#define SNDFX_DSNDDUTY 14
#define SNDFX_DSNDSAW 15
#define SNDFX_DSOUND_PITCH 16
*/

struct ccValExplStruct lfoTargetExpl[]={
	{8,			"C1 duty"},
	{16,		"C3 duty"},
	{24,		"N bits"},
	{32,		"C2 samp"},
	{40,		"C1 pitch"},
	{48,		"C2 pitch"},
	{56,		"N pitch"},
	{64,		"C3 pitch"},
	{72,		"Sw val"},
	{80,		"Sw spd"},
	{88,		"Sw len"},
	{96,		"Sw env"},
	{104,		"C4 sq"},
	{112,		"C4 duty"},
	{120,		"C4 saw"},
	{128,		"C4 pitch"},
	{0, NULL}
};


struct ccValExplStruct waveformExpl[]={
	{0x10,	"square"},
	{0x20,	"triangle"},
	{0x30,	"saw"},
	{0x40,	"2xsquare"},
	{0x50,	"sq gltch"},
	{0x60,	"harmon"},
	{0x70,	"squa alt"},
	{128,	"random"},
	{0, NULL}
};

struct ccValExplStruct glissModeExpl[]={
	{32,		"off"},
	{64,		"quickpress"},
	{96,		"legato"},
	{128,		"always"},
	{0, NULL}
};

struct ccValExplStruct autoMeasExpl[]={
	{32,		"off"},
	{64,		"1/4 meas"},
	{96,		"1/2 meas"},
	{128,		"1 meas"},
	{0, NULL}
};

struct ccValExplStruct noiseBitsExpl[]={
	{64,		"11-bit"},
	{128,		"7-bit"},
	{0, NULL}
};

struct ccValExplStruct dutyCycleExpl[]={
	{43,		"12.5%"},
	{85,		"25%"},
	{128,		"50%"},
	{0, NULL}
};

struct CcDefStruct myCcDefs[]={
	{SNDFX_ARPEGGIATOR,	(1<<1)|(1<<2),	12,	"Arp speed",			NULL},
	{SNDFX_SWEEP_VAL, 	(1<<3),			12,	"Sw step",				NULL},
	{SNDFX_DSNDSQUARE,	(1<<4), 		12,	"Sq pct",				NULL},
	{SNDFX_NOISE_BITS,	(1<<10),		12,	"LFSR len",				noiseBitsExpl},
	{SNDFX_DUTYCYCLE,	(1<<1),			13,	"Duty cycle",			dutyCycleExpl},
	{SNDFX_SAMPLE_SETSAMP,	(1<<2),		13,	"Sample type",			waveformExpl},
	{SNDFX_SWEEP_DUTYCYCLE,	(1<<3),		13,	"Duty cycle",			dutyCycleExpl},
	{SNDFX_DSNDDUTY,	(1<<4),			13,	"Sq duty",				NULL},
	{SNDFX_SWEEP_SPEED,	(1<<3), 		14,	"Sw speed",				NULL},
	{SNDFX_DSNDSAW,		(1<<4), 		14,	"Saw wave pct",			NULL},
	{SNDFX_DECAY,		(1<<4),			72,	"Decay len",			NULL},
	{SNDFX_ENVELOPE,	(1<<1), 		73,	"Env len",				NULL},
	{SNDFX_SWEEP_ENVELOPE,	(1<<3), 	73,	"Env len",				NULL},
	{SNDFX_NOISE_ENVELOPE,	(1<<10), 	73,	"Env len",				NULL},
	{SNDFX_SNDLEN,		(1<<1), 		74,	"Snd len",				NULL},
	{SNDFX_SAMPLE_SNDLEN,(1<<2), 		74,	"Snd len",				NULL},
	{SNDFX_SWEEP_SNDLEN,(1<<3), 		74,	"Snd len",				NULL},
	{SNDFX_NOISE_SNDLEN,(1<<10), 		74,	"Snd len",				NULL},
	{SNDFX_GLISS_MODE,		(1<<1), 	75,	"Gliss mode",			glissModeExpl},
	{SNDFX_SAMPLE_GLISS_MODE,(1<<2), 	75,	"Gliss mode",			glissModeExpl},
	{SNDFX_GLISS_SPEED,	(1<<1)|(1<<2),	76,	"Gliss len",			NULL},
	{SNDFX_LFO1_TARGET,	0xFFFF,			16,	"LFO1 target",			lfoTargetExpl},
	{SNDFX_LFO1_RANGE,	0xFFFF,			17,	"LFO1 range",			NULL},
	{SNDFX_LFO1_FREQ,	0xFFFF,			18,	"LFO1 freq",			NULL},
	{SNDFX_LFO2_TARGET,	0xFFFF,			19,	"LFO2 target",			lfoTargetExpl},
	{SNDFX_LFO2_RANGE,	0xFFFF,			20,	"LFO2 range",			NULL},
	{SNDFX_LFO2_FREQ,	0xFFFF,			21,	"LFO2 freq",			NULL},
	{SNDFX_LFO3_TARGET,	0xFFFF,			22,	"LFO3 target",			lfoTargetExpl},
	{SNDFX_LFO3_RANGE,	0xFFFF,			23,	"LFO3 range",			NULL},
	{SNDFX_LFO3_FREQ,	0xFFFF,			24,	"LFO3 freq",			NULL},
	{SNDFX_SEQ_SPEED,	0x0,			0,	"Speed",				NULL},
	{SNDFX_SEQ_BPMEAS,	0x0,			0,	"Bts/meas",				NULL},
	{SNDFX_SEQ_NOMEAS,	0x0,			0,	"Measures",				NULL},
	{SNDFX_SEQ_AUTOTO,	0x0,			0,	"Quantize to",		autoMeasExpl},
	{SNDFX_SEQ_AUTOPCT,	0x0,			0,	"Quant rate",			NULL},
	{SNDFX_SEQ_BSPLIT,	0x0,			0,	"Bass split",			NULL},
	{SNDFX_SEQ_BOFF,	0x0,			0,	"Bass offs",			NULL},
	{0, 0, 0, NULL, NULL}
};

struct CcDefStruct *ccDefs=myCcDefs;

const int ccPlusChanToSndfxNo(int cc, int chan) {
	int m=(1<<chan);
	int x=0;
	while (ccDefs[x].desc!=NULL) {
		if ((ccDefs[x].midiChannels&m) && ccDefs[x].ccNo==cc) {
			return ccDefs[x].sndFxNo;
		}
		x++;
	}
	return -1;
}
