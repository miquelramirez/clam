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

#include "AudioFileIn.hxx"
#include "AudioFileOut.hxx"
#include "Oscillator.hxx"
#include "AudioMultiplier.hxx"
#include "EnvelopeGenerator.hxx"
#include <iostream>

using namespace CLAM;


void TestIO(void)
{
	printf("TestAudioFileIO: TestIO: Reading file testosc.aiff, writing tmpL.wav\n");
	AudioFileConfig infilecfg;
	AudioFileConfig outfilecfg;

	infilecfg.SetName("filein");
	infilecfg.SetFilename("testosc.aiff");
	infilecfg.SetFiletype(EAudioFileType::eAIFF);

	outfilecfg.SetName("fileout");
	outfilecfg.SetFilename("tmpL.wav");
	outfilecfg.SetFiletype(EAudioFileType::eWave);

	AudioFileIn infile(infilecfg);
	AudioFileOut outfile(outfilecfg);

	Audio buf;
	buf.SetSize(512);

	/** @todo how are we going to pass samplerate and channels? */
	infile.Start();
	outfile.Start();
	do
	{
		infile.Do(buf);
		outfile.Do(buf);
	} while (!infile.Done());
}

void TestSeek(void)
{
	printf("TestAudioFileIO: TestSeek: Reading file tmp.wav, writing tmp2.wav\n");
	AudioFileConfig infilecfg;
	AudioFileConfig outfilecfg;

	infilecfg.SetName("filein");
	infilecfg.SetFilename("tmp.wav");
	infilecfg.SetFiletype(EAudioFileType::eWave);
	infilecfg.SetChannels(2);

	outfilecfg.SetName("fileout");
	outfilecfg.SetFilename("tmp2.wav");
	outfilecfg.SetFiletype(EAudioFileType::eWave);
	outfilecfg.SetChannels(2);

	AudioFileIn infile(infilecfg);
	AudioFileOut outfile(outfilecfg);

	printf("Setting startframe %d/%d/2 = %d",
		infile.Size(),infile.Channels(),infile.Size()/infile.Channels());

	infilecfg.SetStartFrame(infile.Size()/infile.Channels()/2);
	infile.Configure(infilecfg);

	Audio bufL;
	Audio bufR;
	bufL.SetSize(512);
	bufR.SetSize(512);

	/** @todo how are we going to pass samplerate and channels? */
	infile.Start();
	outfile.Start();
	do
	{
		infile.Do(bufL,bufR);
		outfile.Do(bufL,bufR);
	} while (!infile.Done());
}


void TestSplit(void)
{
	printf("TestAudioFileIO: TestSplit: Reading file tmp.wav, writing tmpL.wav,tmpR.wav\n");
	AudioFileConfig infilecfg;
	AudioFileConfig outfilecfgL,outfilecfgR;

	infilecfg.SetName("filein");
	infilecfg.SetFilename("tmp.wav");
	infilecfg.SetFiletype(EAudioFileType::eWave);

	outfilecfgL.SetName("fileoutL");
	outfilecfgL.SetFilename("tmpL.wav");
	outfilecfgL.SetFiletype(EAudioFileType::eWave);

	outfilecfgR.SetName("fileoutR");
	outfilecfgR.SetFilename("tmpR.wav");
	outfilecfgR.SetFiletype(EAudioFileType::eWave);

	AudioFileIn infile(infilecfg);
	AudioFileOut outfileL(outfilecfgL);
	AudioFileOut outfileR(outfilecfgR);

	Audio bufL;
	Audio bufR;
	bufL.SetSize(512);
	bufR.SetSize(512);

	infile.Start();
	outfileL.Start();
	outfileR.Start();
	
	do
	{
		infile.Do(bufL,bufR);
		outfileL.Do(bufL);
		outfileR.Do(bufR);
	} while (!infile.Done());

	infile.Stop();
	outfileL.Stop();
	outfileR.Stop();

}

void TestJoin(void)
{
	printf("TestAudioFileIO: TestJoin: Reading file tmpL.wav, tmpR.wav, writing tmp.wav\n");

	AudioFileConfig outfilecfg;
	AudioFileConfig infilecfgL,infilecfgR;

	outfilecfg.SetName("fileout");
	outfilecfg.SetFilename("tmp.wav");
	outfilecfg.SetChannels(2);
	outfilecfg.SetFiletype(EAudioFileType::eWave);

	infilecfgL.SetName("fileinL");
	infilecfgL.SetFilename("tmpL.wav");
	infilecfgL.SetFiletype(EAudioFileType::eWave);

	infilecfgR.SetName("fileinR");
	infilecfgR.SetFilename("tmpR.wav");
	infilecfgR.SetFiletype(EAudioFileType::eWave);

	AudioFileOut outfile(outfilecfg);
	AudioFileIn infileL(infilecfgL);
	AudioFileIn infileR(infilecfgR);

	Audio bufL;
	Audio bufR;
	bufL.SetSize(512);
	bufR.SetSize(512);

	outfile.Start();
	infileL.Start();
	infileR.Start();
	
	do
	{
		infileL.Do(bufL);
		infileR.Do(bufR);
		outfile.Do(bufL,bufR);
	} while (!(infileL.Done() || infileR.Done()));
	
	outfile.Stop();
	infileL.Stop();
	infileR.Stop();
	
}


/*
void testgen(void)
{
	AudioConfig cfg;
	cfg.SetSize(0); // we want an empty buffer of audio because it will map 
	// a loop owned in for the AudioGenerator
	Audio generated(cfg); // generated Audio object 

	CLAMTest::AudioGeneratorConfig acfg;
	acfg.SetSampleRate(44100);
	acfg.SetFrameSize(256);
	acfg.SetName("Audio_Generator");

	CLAMTest::AudioGenerator audioGen(acfg);
	
	AudioFileConfig outfilecfg;
	outfilecfg.SetName("genfileout");
	outfilecfg.SetFilename("-");
	outfilecfg.SetFiletype(EAudioFileType::eWave);

	AudioFileOut outfile(outfilecfg);

	for (int i=0;i<1024;i++)
	{
		audioGen.Do(generated);
		outfile.Do(generated);
	}
}
*/

void TestOSC(void)
{
	printf("TestAudioFileIO: TestOSC: Creating file testosc.aiff\n");
	Audio bufA,bufB,bufC,bufD; // generated Audio object 

	OscillatorConfig carcfg;
	OscillatorConfig modcfg;
	OscillatorConfig ampmodcfg;
	AudioMultiplier mul;

	bufA.SetSize(256);
	bufB.SetSize(256);
	bufC.SetSize(256);
	bufD.SetSize(256);

	carcfg.SetFrequency(440.);
	
	modcfg.SetFrequency(0.5);
	ampmodcfg.SetFrequency(4);

	modcfg.SetPhase(TData(-PI)/TData(2.));
	carcfg.SetModIndex(440./0.5);

	Oscillator car(carcfg),mod(modcfg),ampmod(ampmodcfg);

	AudioFileConfig outfilecfg;
	
	outfilecfg.SetName("genfileout");
	outfilecfg.SetFilename("testosc.aiff");
	outfilecfg.SetFiletype(EAudioFileType::eAIFF);

	AudioFileOut outfile(outfilecfg);

	mod.Start();
	car.Start();
	ampmod.Start();
	mul.Start();
	outfile.Start();

	for (int i=0;i<200;i++) {
		mod.Do(bufA);
		car.Do(bufA,0,bufB);
		ampmod.Do(bufC);
		mul.Do(bufB,bufC,bufD);
		outfile.Do(bufD);
	}

	mod.Stop();
	car.Stop();
	ampmod.Stop();
	mul.Stop();
	outfile.Stop();

}


void TestEnv(void)
{
	printf("TestAudioFileIO: TestEnv: Creating file tmpR.wav\n");
	Audio buf,bufenv,bufenv2,bufout;

	AudioFileConfig infilecfg;

	OscillatorConfig osccfg;
	EnvelopeGeneratorConfig envgencfg;
	EnvelopeGeneratorConfig envgencfg2;
	EnvelopeConfig envcfg;

	AudioMultiplier mul;

	envcfg.SetBPFSize(3);
	Envelope env(envcfg),env2(envcfg);

	BPFTmpl<TTime,TData> bpf;
	bpf.Insert(0.0,0.0);
	bpf.Insert(500.0,1.0);
	bpf.Insert(1000.0,0.0);
	env.SetAmplitudeBPF(bpf);

	BPFTmpl<TTime,TData> bpf2;
	bpf2.Insert(0.0,1.0);
	bpf2.Insert(500.0,2.0);
	bpf2.Insert(1000.0,0.5);
	env2.SetAmplitudeBPF(bpf2);

	osccfg.SetFrequency(440.);
	osccfg.SetModIndex(1.0);
	Oscillator osc(osccfg);
	envgencfg.SetDuration(4);
	envgencfg2.SetDuration(4);
	EnvelopeGenerator envgen(envgencfg);
	EnvelopeGenerator envgen2(envgencfg2);

	AudioFileConfig outfilecfg;
	
	outfilecfg.SetName("genfileout");
	outfilecfg.SetFilename("tmpR.wav");
	outfilecfg.SetFiletype(EAudioFileType::eWave);

	AudioFileOut outfile(outfilecfg);

	bufenv.SetSize(256);
	bufenv2.SetSize(256);
	buf.SetSize(256);
	bufout.SetSize(256);

	envgen.Start();
	envgen2.Start();
	osc.Start();
	mul.Start();
	outfile.Start();

	for (int i=0; i<200; i++) {
		envgen.Do(env,bufenv);
		envgen2.Do(env2,bufenv2);
		osc.Do(bufenv2,0,buf);
		mul.Do(buf,bufenv,bufout);
		outfile.Do(bufout);
	};

	envgen.Stop();
	envgen2.Stop();
	osc.Stop();
	mul.Stop();
	outfile.Stop();

}

int main(int argc,char** argv)
{
	try {
		TestOSC();
		TestIO();
		TestEnv();
		TestJoin();
		TestSplit();
		TestSeek();
	}	
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
	
	return 0;
}
	
