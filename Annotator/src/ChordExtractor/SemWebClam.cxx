/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

/* 
David Garcia Garzon
Universitat Pompeu Fabra

Jun Wang
Peking University

Yves Raimond
Queen Mary, University of London

Adapted from Python code of GNAT by Yves Raimond at Queen Mary
*/
#include <iostream>
#include <fstream>
//#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#undef _DEBUG
#include <Python.h>
#define _DEBUG

#include "ChordExtractor.hxx"
#include "MonoAudioFileReader.hxx"
#include "AudioInPort.hxx"
#include "AudioOutPort.hxx"
#include "XMLStorage.hxx"
#include "Pool.hxx"
#include "Array.hxx"
#include "Enumerated.hxx"
#include "FrameDivision.hxx"
#include "CLAM/DiscontinuousSegmentation.hxx"
#include <CLAM/Assert.hxx>

const char * copyright =
	"SemWeb extraction v1.0.\n"
	"Copyright 2008 Universitat Pompeu Fabra\n"
	"Original algorithm by Yves Raimond.\n"
	"Ported to C++ by Jun Wang and David Garcia Garzon.\n"
	"\n"
	;
const char * usage =
	"Usage: SemWebExtractor [-s out.sc] [-f <suffix>] [-m <method>] <wavefile1> <wavefile2> ...\n"
	"\nOptions:\n"
	" -h            shows this help\n"
	" -s            dump the schema to the standard output\n"
	" -f <suffix>   append <suffix> to the generated descriptors file (default: '.pool')\n"
	" -m <method>   use the <method> segmentation algorithm\n"
	"\nUsage examples:\n"
	" SemWebExtractor -s schema.sc\n"
	" SemWebExtractor -f .beats song1.wav song2.mp3 song3.ogg\n"
	;
const char * schemaContent = 
"<?xml version='1.0' encoding='UTF-8' standalone='no' ?>\n"
"<DescriptionScheme>\n"
"\n"
" <Uri>descriptionScheme:www.iua.upf.edu:clam:SemWebExtraction</Uri>\n"
"\n"
" <Attributes>\n"
"   <Attribute name='Artist' scope='Song' type='String'/>\n"
"   <Attribute name='Album' scope='Song' type='String'/>\n"
"   <Attribute name='Track-No' scope='Song' type='String'/>\n"
"   <Attribute name='Title' scope='Song' type='String'/>\n"
"   <Attribute name='Date' scope='Song' type='String'/>\n"
"   <Attribute name='Lyrics' scope='Song' type='String'/>\n"
"   <Attribute name='Songlist-URI' scope='Song' type='String'/>\n"
"   <Attribute name='Photo-URI' scope='Song' type='String'/>\n"
"   <Attribute name='Tag-Jamendo' scope='Song' type='String'/>\n"
"   <Attribute name='Tag-MusicBrainz' scope='Song' type='String'/>\n"
"   <Attribute name='Tag-SemWeb' scope='Song' type='String'/>\n"
"   <Attribute name='Album-Description' scope='Song' type='String'/>\n"
"   <Attribute name='Review' scope='Song' type='String'/>\n"
"   <Attribute name='Genre' scope='Song' type='String'/>\n"
"   <Attribute name='Albumlist-URI' scope='Song' type='String'/>\n"
"   <Attribute name='Artist-Photo' scope='Song' type='String'/>\n"
"   <Attribute name='Artist-Homepage-URI' scope='Song' type='String'/>\n"
"   <Attribute name='Artist-Location-URI' scope='Song' type='String'/>\n"
"   <Attribute name='Album-Rating' scope='Song' type='String'/>\n"
"   <Attribute name='Songlist' scope='Song' type='String'/>\n"

"  </Attributes>\n"
"\n"
"</DescriptionScheme>\n"
;

class SemWebDescriptionDumper
{
	std::list<std::string>  _extractor; 
	CLAM::DescriptionScheme _schema;
	CLAM::DescriptionDataPool * _pool;

public:
	SemWebDescriptionDumper()
	{
		_schema.AddAttribute<CLAM::Text>("Song", "Artist");
		_schema.AddAttribute<CLAM::Text>("Song", "Album");
		_schema.AddAttribute<CLAM::Text>("Song", "Track-No");
		_schema.AddAttribute<CLAM::Text>("Song", "Title");
		_schema.AddAttribute<CLAM::Text>("Song", "Date");
		_schema.AddAttribute<CLAM::Text>("Song", "Lyrics");
		_schema.AddAttribute<CLAM::Text>("Song", "Songlist-URI");
		_schema.AddAttribute<CLAM::Text>("Song", "Photo-URI");
		_schema.AddAttribute<CLAM::Text>("Song", "Tag-Jamendo");
		_schema.AddAttribute<CLAM::Text>("Song", "Tag-MusicBrainz");
		_schema.AddAttribute<CLAM::Text>("Song", "Tag-SemWeb");
		_schema.AddAttribute<CLAM::Text>("Song", "Album-Description");
		_schema.AddAttribute<CLAM::Text>("Song", "Review");
		_schema.AddAttribute<CLAM::Text>("Song", "Genre");
		_schema.AddAttribute<CLAM::Text>("Song", "Albumlist-URI");
		_schema.AddAttribute<CLAM::Text>("Song", "Artist-Photo");
		_schema.AddAttribute<CLAM::Text>("Song", "Artist-Homepage-URI");
		_schema.AddAttribute<CLAM::Text>("Song", "Artist-Location-URI");
		_schema.AddAttribute<CLAM::Text>("Song", "Album-Rating");
		_schema.AddAttribute<CLAM::Text>("Song", "Songlist");
		
		_pool = new CLAM::DescriptionDataPool(_schema);
		_pool->SetNumberOfContexts("Song", 1);		
		}
	
	const CLAM::DescriptionDataPool & getPool() const
	{
		return *_pool;
	}
	
	void doIt()
	{	// Write Song level descriptors	
		std::cout<<"temp_1 is processing-----------------   "<<std::endl;
		getData("temp_1", _pool->GetWritePool<CLAM::Text>("Song","Title"));
		getData("temp_2", _pool->GetWritePool<CLAM::Text>("Song","Track-No"));
		getData("temp_3", _pool->GetWritePool<CLAM::Text>("Song","Lyrics"));
		getData("temp_4", _pool->GetWritePool<CLAM::Text>("Song","Genre"));
		getData("temp_5", _pool->GetWritePool<CLAM::Text>("Song","Album-Rating"));
		getData("temp_6", _pool->GetWritePool<CLAM::Text>("Song","Tag-MusicBrainz"));
		getData("temp_7", _pool->GetWritePool<CLAM::Text>("Song","Album"));
		getData("temp_8", _pool->GetWritePool<CLAM::Text>("Song","Date"));
		getData("temp_9", _pool->GetWritePool<CLAM::Text>("Song","Photo-URI"));
		getData("temp_10", _pool->GetWritePool<CLAM::Text>("Song","Songlist-URI"));
		getData("temp_11", _pool->GetWritePool<CLAM::Text>("Song","Tag-SemWeb"));
		getData("temp_12", _pool->GetWritePool<CLAM::Text>("Song","Songlist"));
		getData("temp_13", _pool->GetWritePool<CLAM::Text>("Song","Album-Description"));
		getData("temp_14", _pool->GetWritePool<CLAM::Text>("Song","Review"));
		getData("temp_15", _pool->GetWritePool<CLAM::Text>("Song","Tag-Jamendo"));
		getData("temp_16", _pool->GetWritePool<CLAM::Text>("Song","Artist"));
		getData("temp_17", _pool->GetWritePool<CLAM::Text>("Song","Artist-Location-URI"));
		getData("temp_18", _pool->GetWritePool<CLAM::Text>("Song","Artist-Homepage-URI"));
		getData("temp_19", _pool->GetWritePool<CLAM::Text>("Song","Artist-Photo"));
		getData("temp_20", _pool->GetWritePool<CLAM::Text>("Song","Albumlist-URI"));
	}

	void getData(const char*  tempFile, CLAM::Text * metadata)
	{
		FILE * fp;
		char* str;
		long lSize, result;
		
		if(fp=fopen(tempFile,"rb"))
		{
			fseek (fp , 0 , SEEK_END);
			lSize = ftell (fp);
			fseek(fp, 0, SEEK_SET);
			str = (char*) malloc (sizeof(char)*(lSize+1));
			if (str == NULL) 
				{
				std::cerr<<"Memory error"<<std::endl;
				return;
				}
			result = fread (str, sizeof(char), lSize, fp);
			str[lSize]='\0';  //I hate coding I hate coding I hate coding I hate coding I hate coding I hate coding I hate coding I hate coding
			if (result != lSize) 
				{
				std::cerr<<"Reading error"<<std::endl;
				return;
				}
			
			metadata[0] = str;
			std::cout<<"the content is: "<<str<<std::endl;
			free(str);
			fclose(fp);
			if(remove(tempFile)!=0)
				std::cerr << "Cant delete file:" << tempFile<<std::endl;	
		}
		else
			{
				std::cout<<"No such file or directory:"<<tempFile<<std::endl;
			}
	}
	
	~SemWebDescriptionDumper()
	{
		delete _pool;
	}	
};

int processFile(const std::string & waveFile, const std::string & suffix, unsigned segmentationMethod)
{
	PyObject* pMod = NULL;
	PyObject* pParm = NULL;
	PyObject* pRetVal = NULL;
	PyObject* pDict = NULL;
	PyObject* pClass = NULL;
	PyObject* pInstance = NULL;
	std::list<std::string>  metadata;
	int   iRetVal = 0;

	//introduce the python module
	pMod = PyImport_ImportModule("SemWebCrawler"); //jun: obsolute path!!!
	if(!pMod)
		{
		std::cerr << "Cant open python file!" << std::endl;
		return -1;
		}
	pDict = PyModule_GetDict(pMod);
	if(!pDict)
		{
		std::cerr<< "Cant find dictionary!"<< std::endl;
		return -1;
		}
	
	std::cout<<"the input of waveFile is:   "<<waveFile<<std::endl;
	//create 1 parameter
	pParm = PyTuple_New(1);
	std::cout<<"0-----pParm is initialed:   "<<std::endl;
	pClass = PyDict_GetItemString(pDict, "SemWebCrawler");
	std::cout<<"1-----pClass is done:   "<<std::endl;

	PyTuple_SetItem(pParm, 0, Py_BuildValue("s",waveFile.c_str()));
	std::cout<<"1-----pParm is done:   "<<std::endl;

	pInstance = PyInstance_New(pClass, pParm, NULL);   
	std::cout<<"3-----pInstance is done:   "<<std::endl;
	pRetVal=PyObject_CallMethod(pInstance, "spider", NULL);
	std::cout<<"4-----pRetVal is done:   "<<std::endl;
	//PyArg_Parse(pRetVal, "(sss)", &testA, &testB, &testC);
	/*std::list<std::string>::iterator it = metadata.begin();*/
	Py_DECREF(pMod);
	std::cout<<"6-----py_DECREF is done:   "<<std::endl;
	
	//PyAPI_FUNC(PyObject *) PyString_FromString(const char *);
	SemWebDescriptionDumper dumper;
	std::cout<<"7-----dumper initiate is done:   "<<std::endl;
	dumper.doIt();
	std::cout<<"8-----dumper.doIt is done:   "<<std::endl;
	std::ofstream outputPool((waveFile+suffix).c_str());
	std::cout<<"9-----outputpool waveFile+suffix is ready:   "<<std::endl;
	//testPool=dumper.getPool();
	//std::cout<<testPool.GetReadPool<CLAM::Text>("Song","Review")<<std::endl;
	CLAM::XMLStorage::Dump(dumper.getPool(), "Description", outputPool);
	std::cout<<"10-----pool is done:   "<<std::endl;
	return 0;
}



int main(int argc, char* argv[])			// access command line arguments
{
	Py_Initialize();
	if(!Py_IsInitialized())
		return -1;
	PyRun_SimpleString("import sys\nprint sys.path\nprint sys.version\n");
	PyRun_SimpleString("sys.path.append('C:\\Jun\\devel\\Annotator\\src\\SemWebExtractor')");
	PyRun_SimpleString("\n\nprint sys.path\nprint sys.version\n");
	
	std::cout << copyright << std::endl;
	std::list<std::string> songs;
	std::string suffix = ".pool";
	std::string schemaLocation = "";
	unsigned segmentationMethod = 0;
	if (argc==1) 
	{
		std::cerr << usage << std::endl;
		return -1;
	}

	bool hasSchemaOption = false;
	bool isSchema = false;
	bool isSuffix = false;
	bool isConfiguration = false;
	bool isSegmentationMethod = false;
	for (unsigned i = 1; i<argc; i++)
	{
		std::string parameter = argv[i];
		if (parameter=="-h")
		{
			std::cerr << usage << std::endl;
			return -1;
		}
		else if (isSchema)
		{
			schemaLocation = parameter;
			isSchema=false;
		}
		else if (isSuffix)
		{
			suffix = parameter;
			isSuffix=false;
		}
		else if (isSegmentationMethod)
		{
			segmentationMethod = atoi( argv[i] );
			isSegmentationMethod=false;
		}
		else if (isConfiguration)
		{
			// TODO: Take the configuration file
			isConfiguration=false;
		}
		else if (parameter=="-f") isSuffix = true;
		else if (parameter=="-s") isSchema = hasSchemaOption = true;
		else if (parameter=="-m") isSegmentationMethod = true;
		else if (parameter=="-c") isConfiguration = true;
		else if (parameter=="-w") return 0; // No write back
		else songs.push_back(parameter);
	}

	if (schemaLocation!="")
	{
		std::ofstream schemaFile(schemaLocation.c_str());
		schemaFile << schemaContent;
	}

	for (std::list<std::string>::iterator it = songs.begin();
			it!= songs.end();
			it++)
	{
		int error = processFile(*it, suffix, segmentationMethod);
		if (error) return error;
	}

	Py_Finalize();
	return 0;
}


