/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


#include "RappidConfig.hxx"
#include <getopt.h>


void RappidConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	GetViolaIOConfig().SetName("Viola");
	GetViolaIOConfig().SetAudioDeviceChannelID(0);
	GetHarpIOConfig().SetName("Harp");
	GetHarpIOConfig().SetAudioDeviceChannelID(1);
	GetSynthLeftIOConfig().SetName("Synth_Left");
	GetSynthLeftIOConfig().SetAudioDeviceChannelID(0);
	GetSynthRightIOConfig().SetName("Synth_Right");
	GetSynthRightIOConfig().SetAudioDeviceChannelID(1);
	GetEnvMorph1Cfg().SetName("EnvelopeMorpher1");
	GetEnvMorph2Cfg().SetName("EnvelopeMorpher2");
	SetTimeLimit(0.0);
	SetRealTimeSched(false);
	SetRealTimePriority(50);
	SetDoControlTest(false);
	SetControlTestSleep(10.0);
	SetDebugSignal(eNoSignal);
}

void RappidConfig::Synchronise(void)
{
	GetViolaIOConfig().SetGlobalConfig(GetGlobalConfig());
	GetHarpIOConfig() .SetGlobalConfig(GetGlobalConfig());
	GetSynthLeftIOConfig().SetGlobalConfig(GetGlobalConfig());
	GetSynthRightIOConfig().SetGlobalConfig(GetGlobalConfig());
	GetEnvMorph1Cfg() .SetGlobalConfig(GetGlobalConfig());
	GetEnvMorph2Cfg() .SetGlobalConfig(GetGlobalConfig());
}

void RappidConfig::Dump(void)
{
	Synchronise();
	std::cout << "  Name:                    ";
	std::cout << GetName();
	std::cout << std::endl;
	std::cout << "  Global Configuration:";
	std::cout << std::endl;
	GetGlobalConfig().Dump();
	GetViolaIOConfig().Dump();
	GetHarpIOConfig().Dump();
	GetSynthLeftIOConfig().Dump();
	GetSynthRightIOConfig().Dump();
	std::cout << "  Configuration for EnvelopeMorpher1: ";
	std::cout << std::endl;
	GetEnvMorph1Cfg().Dump();
}

void RappidConfig::parse_command_line(int argc, char *argv[])
{
	int c,option_index;
	std::stringstream sarg;
	std::string store_file;
	TData fval;
	bool store = false;

	static struct option long_options[] = {
		{"test-sleep",           1, 0,  500}, //General
		{"test",                 0, 0,  501},
		{"verbose",              0, 0,  502},
		{"help",                 0, 0,  503},
		{"config-file",          1, 0,  504},
		{"store-config-file",    1, 0,  505},
		{"adat",                 0, 0,  506},

		{"viola-file",           1, 0, 1000}, //Viola
		{"viola-device",         1, 0, 1001},
		{"viola-channel",        1, 0, 1002},
		{"viola-wave",           0, 0, 1003},
						 							 
		{"harp-file",            1, 0, 2000}, //Harp
		{"harp-device",          1, 0, 2001},
		{"harp-channel",         1, 0, 2002},
		{"harp-wave",            0, 0, 2003},
						 							 
		{"synth-left-file",      1, 0, 3000}, //Output Left
		{"synth-left-device",    1, 0, 3001},
		{"synth-left-channel",   1, 0, 3002},

		{"synth-right-file",     1, 0, 4000}, //Output Right
		{"synth-right-device",   1, 0, 4001},
		{"synth-right-channel",  1, 0, 4002},

		{"em1-envelope-file",    1, 0, 10000}, //Envelope Morpher 1
		{"em1-interp-period",    1, 0, 10001},
		{"em1-envelope-mem",     1, 0, 10002},
		{"em1-normal-level",     1, 0, 10003},
		{"em1-silence-level",    1, 0, 10004},
		{"em1-interp-type",      1, 0, 10005},

		{"em2-envelope-file",    1, 0, 10010}, //Envelope Morpher 1
		{"em2-interp-period",    1, 0, 10011},
		{"em2-envelope-mem",     1, 0, 10012},
		{"em2-normal-level",     1, 0, 10013},
		{"em2-silence-level",    1, 0, 10014},
		{"em2-interp-type",      1, 0, 10015},

		{0, 0, 0, 0}
	};

	SetName("RappidApplication");

	bool viola_set = false, harp_set = false, synthl_set = false, synthr_set = false;

	while ((c = getopt_long (argc, argv, "hvs:t:r:", long_options, &option_index)) != -1) {
		if (optarg)
			sarg.str(optarg);
		else
			sarg.str("");
		switch (c)
		{
		case 'h':
		case 503:
			print_command_line_help();
			exit(0);
		case 502:
		case 'v':
			GetGlobalConfig().SetVerbose(true);
			break;
		case 's':
			GetGlobalConfig().SetSampleRate(atoi(optarg));
			break;
		case 't':
			SetTimeLimit(atoi(optarg));
			break;
		case 'r':
			SetRealTimeSched(true);
			SetRealTimePriority(atoi(optarg));
			break;
		case 500:
			SetControlTestSleep(atoi(optarg));
		case 501:
			SetDoControlTest(true);
			break;
		case 504:
			CLAM::XMLStorage::Restore(*this,optarg);
			break;
		case 505:
			store=true;
			store_file = optarg;
			break;
		case 506:
			GetGlobalConfig().SetADAT(true);
			break;

			// VIOLA
		case 1000:
			if (viola_set && 
				GetViolaIOConfig().GetIOType() != ERappidIOType::eFile) {
				std::cerr << "Multiple incompatible viola input types requested.\n";
				exit(1);
			}
			GetViolaIOConfig().SetFileName(optarg);
			GetViolaIOConfig().SetIOType(ERappidIOType::eFile);
			viola_set = true;
			break;
		case 1001:
			if (viola_set && 
				GetViolaIOConfig().GetIOType() != ERappidIOType::eRealTime) {
				std::cerr << "Multiple incompatible viola input types requested.\n";
				exit(1);
			}
			GetViolaIOConfig().SetAudioDevice(optarg);
			GetViolaIOConfig().SetIOType(ERappidIOType::eRealTime);
			viola_set = true;
			break;
		case 1002:
			if (viola_set && 
				GetViolaIOConfig().GetIOType() != ERappidIOType::eRealTime) {
				std::cerr << "Multiple incompatible viola input types requested.\n";
				exit(1);
			}
			GetViolaIOConfig().SetAudioDeviceChannelID(atoi(optarg));
			GetViolaIOConfig().SetIOType(ERappidIOType::eRealTime);
			viola_set = true;
			break;
		case 1003:
			if (viola_set && 
				GetViolaIOConfig().GetIOType() != ERappidIOType::eWaveform) {
				std::cerr << "Multiple incompatible viola input types requested.\n";
				exit(1);
			}
			GetViolaIOConfig().SetIOType(ERappidIOType::eWaveform);
			viola_set = true;
			break;

			// HARP
		case 2000:
			if (harp_set && 
				GetHarpIOConfig().GetIOType() != ERappidIOType::eFile) {
				std::cerr << "Multiple incompatible harp input types requested.\n";
				exit(1);
			}
			GetHarpIOConfig().SetFileName(optarg);
			GetHarpIOConfig().SetIOType(ERappidIOType::eFile);
			harp_set = true;
			break;
		case 2001:
			if (harp_set && 
				GetHarpIOConfig().GetIOType() != ERappidIOType::eRealTime) {
				std::cerr << "Multiple incompatible harp input types requested.\n";
				exit(1);
			}
			GetHarpIOConfig().SetAudioDevice(optarg);
			GetHarpIOConfig().SetIOType(ERappidIOType::eRealTime);
			harp_set = true;
			break;
		case 2002:
			if (harp_set && 
				GetHarpIOConfig().GetIOType() != ERappidIOType::eRealTime) {
				std::cerr << "Multiple incompatible harp input types requested.\n";
				exit(1);
			}
			GetHarpIOConfig().SetAudioDeviceChannelID(atoi(optarg));
			GetHarpIOConfig().SetIOType(ERappidIOType::eRealTime);
			harp_set = true;
			break;
		case 2003:
			if (harp_set && 
				GetHarpIOConfig().GetIOType() != ERappidIOType::eWaveform) {
				std::cerr << "Multiple incompatible harp input types requested.\n";
				exit(1);
			}
			GetHarpIOConfig().SetIOType(ERappidIOType::eWaveform);
			harp_set = true;
			break;

			// OUTPUT LEFT
		case 3000:
			if (synthl_set && 
				GetSynthLeftIOConfig().GetIOType() != ERappidIOType::eFile)
				GetSynthLeftIOConfig().SetIOType(ERappidIOType::eRealTimeAndFile);
			else
				GetSynthLeftIOConfig().SetIOType(ERappidIOType::eFile);
			GetSynthLeftIOConfig().SetFileName(optarg);
			synthl_set = true;
			break;
		case 3001:
			if (synthl_set && 
				GetSynthLeftIOConfig().GetIOType() != ERappidIOType::eRealTime)
				GetSynthLeftIOConfig().SetIOType(ERappidIOType::eRealTimeAndFile);
			else
				GetSynthLeftIOConfig().SetIOType(ERappidIOType::eRealTime);
			GetSynthLeftIOConfig().SetAudioDevice(optarg);
			synthl_set = true;
			break;
		case 3002:
			if (synthl_set && 
				GetSynthLeftIOConfig().GetIOType() != ERappidIOType::eRealTime)
				GetSynthLeftIOConfig().SetIOType(ERappidIOType::eRealTimeAndFile);
			else
				GetSynthLeftIOConfig().SetIOType(ERappidIOType::eRealTime);
			GetSynthLeftIOConfig().SetAudioDeviceChannelID(atoi(optarg));
			synthl_set = true;
			break;

			// OUTPUT RIGHT
		case 4000:
			if (synthr_set && 
				GetSynthRightIOConfig().GetIOType() != ERappidIOType::eFile)
				GetSynthRightIOConfig().SetIOType(ERappidIOType::eRealTimeAndFile);
			else
				GetSynthRightIOConfig().SetIOType(ERappidIOType::eFile);
			GetSynthRightIOConfig().SetFileName(optarg);
			synthr_set = true;
			break;
		case 4001:
			if (synthr_set && 
				GetSynthRightIOConfig().GetIOType() != ERappidIOType::eRealTime)
				GetSynthRightIOConfig().SetIOType(ERappidIOType::eRealTimeAndFile);
			else
				GetSynthRightIOConfig().SetIOType(ERappidIOType::eRealTime);
			GetSynthRightIOConfig().SetAudioDevice(optarg);
			synthr_set = true;
			break;
		case 4002:
			if (synthr_set && 
				GetSynthRightIOConfig().GetIOType() != ERappidIOType::eRealTime)
				GetSynthRightIOConfig().SetIOType(ERappidIOType::eRealTimeAndFile);
			else
				GetSynthRightIOConfig().SetIOType(ERappidIOType::eRealTime);
			GetSynthRightIOConfig().SetAudioDeviceChannelID(atoi(optarg));
			synthr_set = true;
			break;

			// ENVELOPE MORPHER 1
		case 10000:
			GetEnvMorph1Cfg().SetEnvelopeFile(optarg);
			break;
		case 10001:
			sarg >> fval;
			GetEnvMorph1Cfg().GetInterpolationPeriod().SetInitValue(fval);
			break;
		case 10002:
			sarg >> fval;
			GetEnvMorph1Cfg().GetIntegrationLength().SetInitValue(fval);
			break;
		case 10003:
			sarg >> fval;
			GetEnvMorph1Cfg().GetNormalLevel().SetInitValue(fval);
			break;
		case 10004:
			sarg >> fval;
			GetEnvMorph1Cfg().GetSilenceLevel().SetInitValue(fval);
			break;
		case 10005:
			GetEnvMorph1Cfg().SetInterpolationType(EInterpolation(optarg));
			break;

			// ENVELOPE MORPHER 2
		case 10010:
			GetEnvMorph2Cfg().SetEnvelopeFile(optarg);
			break;
		case 10011:
			sarg >> fval;
			GetEnvMorph2Cfg().GetInterpolationPeriod().SetInitValue(fval);
			break;
		case 10012:
			sarg >> fval;
			GetEnvMorph2Cfg().GetIntegrationLength().SetInitValue(fval);
			break;
		case 10013:
			sarg >> fval;
			GetEnvMorph2Cfg().GetNormalLevel().SetInitValue(fval);
			break;
		case 10014:
			sarg >> fval;
			GetEnvMorph2Cfg().GetSilenceLevel().SetInitValue(fval);
			break;
		case 10015:
			GetEnvMorph2Cfg().SetInterpolationType(EInterpolation(optarg));
			break;

		case '?':
			std::cerr << "Unknown option: -" << optopt << std::endl;
			exit(1);
		default:
			CLAM_ASSERT(false,"Impossible option");
		}
	}

	Synchronise();

	if (GetGlobalConfig().GetVerbose()) {
		std::cout << "Rappid configuration:" << std::endl;
		Dump();
	}
	if (store)
		CLAM::XMLStorage::Dump(*this,"Rappid_configuration",store_file);
}

void RappidConfig::print_command_line_help()
{
	int i;
	std::cout <<
		"Usage:\n"
		"  Rappid  [-h] [-v] [-s sample_rate] [-r priority] \n"
		"          [--help] [--verbose] \n"
		"          [--config-file f ] [--store-config-file f] \n"
		"          [ [--viola-file f ] | [ [--viola-device d ] [--viola-channel c ] ] ]\n"
		"          [ [--harp-file  f ] | [ [--harp-device  d ] [--harp-channel  c ] ] ]\n"
		"          [ [--synth-file f ] | [ [--synth-device d ] [--synth-channel c ] ] ]\n"
		"          [ --emN-interp-period v ] [ --emN-envelope-mem v ]\n"
		"          [ --emN-normal-level  v ] [ --emN-silence-level v ]\n"
		"          [ --emN-interp-type v   ]\n"
		"\n"
		" General options:\n"
		"\n"
		"  -h, --help:       Shows this help message.\n"
		"  -v:               Be verbose.\n"
		"  -r p              Use real time scheduling (SCHED_RR) with priority p\n"
		"  -s sample_rate:   Sampling rate for input and output audio streams.\n"
		"  -t exec_time:     Application execution time, in milliseconds.\n"
		"  --test            Run the test controler in background\n"
		"  --test-sleep s    Run the test controler, with an sleep time of s microseconds\n"
		"\n"
		" Input/Output configuration options:\n"
		"\n"
		"  Viola Input:\n"
		"  --viola-device d      Use real time input from device d.\n"
		"  --viola-channel c     Use channel c in the input device.\n"
		"  --viola-file f        Use file input from file f.\n"
		"  --viola-wave          Use synthesized waveform.\n"
		"\n"
		"  Harp Input:\n"
		"  --harp-device d       Use real time input from device d.\n"
		"  --harp-channel c      Use channel c in the input device.\n"
		"  --harp-file f         Use file input from file f.\n"
		"  --harp-wave           Use synthesized waveform.\n"
		"\n"
		"  Synthesized Left Output:\n"
		"  --synth-left-device d  Use real time output into device d.\n"
		"  --synth-left-channel c Use channel c in the output device.\n"
		"  --synth-left-file f    Use file output to file f.\n"
		"\n"
		"  Synthesized Right Output:\n"
		"  --synth-right-device d  Use real time output into device d.\n"
		"  --synth-right-channel c Use channel c in the output device.\n"
		"  --synth-right-file f    Use file output to file f.\n"
		"\n"
		" Envelope Morpher N options: (N = 1 or 2) \n"
		"\n"
		"  --emN-interp-period v  Time (in milliseconds) to use as interpolation period\n"
		"  --emN-envelope-mem v   Time (in milliseconds) to use as envelope integration period\n"
		"  --emN-normal-level v   Average input envelope level which gives unit evelope\n"
		"  --emN-silence-level v  Minimun envelope level which is not truncated to zero.\n"
		"  --emN-interp-type v    Interpolation type, one of:\n";
	i=0;
	while (EInterpolation::sEnumValues[i].name)
		std::cout 
			<< 	"                        " 
			<< EInterpolation::sEnumValues[i++].name << std::endl;
}

