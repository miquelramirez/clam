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

Embed Python code of GNAT by Yves Raimond at Queen Mary
*/
#include <iostream>
#include <fstream>
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
	"Copyright 2009 Universitat Pompeu Fabra\n"
	"An Web-based Extractor Coding by Jun Wang and David Garcia Garzon.\n"
	"Embedding Python code of GNAT by Yves Raimond at Queen Mary\n"	
	"\n"
	;
const char * usage =
	"Usage: SemWebExtractor [-s out.sc] [-f <suffix>] <wavefile1> <wavefile2> ...\n"
	"\nOptions:\n"
	" -h            shows this help\n"
	" -s            dump the schema to the standard output\n"
	" -f <suffix>   append <suffix> to the generated descriptors file (default: '.pool')\n"
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
"   <Attribute name='TrackNo' scope='Song' type='String'/>\n"
"   <Attribute name='Title' scope='Song' type='String'/>\n"
"   <Attribute name='Date' scope='Song' type='String'/>\n"
"   <Attribute name='Lyrics' scope='Song' type='String'/>\n"
"   <Attribute name='SonglistURI' scope='Song' type='String'/>\n"
"   <Attribute name='PhotoURI' scope='Song' type='String'/>\n"
"   <Attribute name='TagJamendo' scope='Song' type='String'/>\n"
"   <Attribute name='TagMusicBrainz' scope='Song' type='String'/>\n"
"   <Attribute name='TagSemWeb' scope='Song' type='String'/>\n"
"   <Attribute name='AlbumDescription' scope='Song' type='String'/>\n"
"   <Attribute name='Review' scope='Song' type='String'/>\n"
"   <Attribute name='Genre' scope='Song' type='String'/>\n"
"   <Attribute name='AlbumlistURI' scope='Song' type='String'/>\n"
"   <Attribute name='ArtistPhoto' scope='Song' type='String'/>\n"
"   <Attribute name='ArtistHomepageURI' scope='Song' type='String'/>\n"
"   <Attribute name='ArtistLocationURI' scope='Song' type='String'/>\n"
"   <Attribute name='AlbumRating' scope='Song' type='String'/>\n"
"   <Attribute name='Songlist' scope='Song' type='String'/>\n"
"   <Attribute name='Usage' scope='Song' type='String'/>\n"
"   <Attribute name='Expression' scope='Song' type='String'/>\n"
"   <Attribute name='Emotion' scope='Song' type='String'/>\n"
"   <Attribute name='DynamicStructure' scope='Song' type='String'/>\n"
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
		_schema.AddAttribute<CLAM::Text>("Song", "TrackNo");
		_schema.AddAttribute<CLAM::Text>("Song", "Title");
		_schema.AddAttribute<CLAM::Text>("Song", "Date");
		_schema.AddAttribute<CLAM::Text>("Song", "Lyrics");
		_schema.AddAttribute<CLAM::Text>("Song", "SonglistURI");
		_schema.AddAttribute<CLAM::Text>("Song", "PhotoURI");
		_schema.AddAttribute<CLAM::Text>("Song", "TagJamendo");
		_schema.AddAttribute<CLAM::Text>("Song", "TagMusicBrainz");
		_schema.AddAttribute<CLAM::Text>("Song", "TagSemWeb");
		_schema.AddAttribute<CLAM::Text>("Song", "AlbumDescription");
		_schema.AddAttribute<CLAM::Text>("Song", "Review");
		_schema.AddAttribute<CLAM::Text>("Song", "Genre");
		_schema.AddAttribute<CLAM::Text>("Song", "AlbumlistURI");
		_schema.AddAttribute<CLAM::Text>("Song", "ArtistPhoto");
		_schema.AddAttribute<CLAM::Text>("Song", "ArtistHomepageURI");
		_schema.AddAttribute<CLAM::Text>("Song", "ArtistLocationURI");
		_schema.AddAttribute<CLAM::Text>("Song", "AlbumRating");
		_schema.AddAttribute<CLAM::Text>("Song", "Songlist");
		_schema.AddAttribute<CLAM::Text>("Song", "Usage");
		_schema.AddAttribute<CLAM::Text>("Song", "Expression");
		_schema.AddAttribute<CLAM::Text>("Song", "Emotion");
		_schema.AddAttribute<CLAM::Text>("Song", "DynamicStructure");
		
		_pool = new CLAM::DescriptionDataPool(_schema);
		_pool->SetNumberOfContexts("Song", 1);		
		}
	
	const CLAM::DescriptionDataPool & getPool() const
	{
		return *_pool;
	}
	
	void doIt()
	{	// Write song-level descriptors	
		getData("temp_1", _pool->GetWritePool<CLAM::Text>("Song","Title"));
		getData("temp_2", _pool->GetWritePool<CLAM::Text>("Song","TrackNo"));
		getData("temp_3", _pool->GetWritePool<CLAM::Text>("Song","Lyrics"));
		getData("temp_4", _pool->GetWritePool<CLAM::Text>("Song","Genre"));
		getData("temp_5", _pool->GetWritePool<CLAM::Text>("Song","AlbumRating"));
		getData("temp_6", _pool->GetWritePool<CLAM::Text>("Song","TagMusicBrainz"));
		getData("temp_7", _pool->GetWritePool<CLAM::Text>("Song","Album"));
		getData("temp_8", _pool->GetWritePool<CLAM::Text>("Song","Date"));
		getData("temp_9", _pool->GetWritePool<CLAM::Text>("Song","PhotoURI"));
		getData("temp_10", _pool->GetWritePool<CLAM::Text>("Song","SonglistURI"));
		getData("temp_11", _pool->GetWritePool<CLAM::Text>("Song","TagSemWeb"));
		getData("temp_12", _pool->GetWritePool<CLAM::Text>("Song","Songlist"));
		getData("temp_13", _pool->GetWritePool<CLAM::Text>("Song","AlbumDescription"));
		getData("temp_14", _pool->GetWritePool<CLAM::Text>("Song","Review"));
		getData("temp_15", _pool->GetWritePool<CLAM::Text>("Song","TagJamendo"));
		getData("temp_16", _pool->GetWritePool<CLAM::Text>("Song","Artist"));
		getData("temp_17", _pool->GetWritePool<CLAM::Text>("Song","ArtistLocationURI"));
		getData("temp_18", _pool->GetWritePool<CLAM::Text>("Song","ArtistHomepageURI"));
		getData("temp_19", _pool->GetWritePool<CLAM::Text>("Song","ArtistPhoto"));
		getData("temp_20", _pool->GetWritePool<CLAM::Text>("Song","AlbumlistURI"));
		_pool->GetWritePool<CLAM::Text>("Song","Usage");
		_pool->GetWritePool<CLAM::Text>("Song","Expression");
		_pool->GetWritePool<CLAM::Text>("Song","Emotion");
		_pool->GetWritePool<CLAM::Text>("Song","DynamicStructure");
	}

	void getData(const char*  tempFile, CLAM::Text * metadata)
	{
		FILE * fp=fopen(tempFile,"rb");
		if(not fp)
		{
			std::cerr << "No such file or directory:" << tempFile << std::endl;
			return;
		}
		fseek (fp , 0 , SEEK_END);
		long lSize = ftell (fp);
		fseek(fp, 0, SEEK_SET);
		char* str = (char*) malloc (sizeof(char)*(lSize+1));
		if (str == NULL) 
		{
			std::cerr << "Memory error" << std::endl;
			return;
		}
		long result = fread (str, sizeof(char), lSize, fp);
		str[lSize]='\0';
		if (result != lSize) 
		{
			std::cerr << "Reading error" << std::endl;
			return;
		}
		
		metadata[0] = str;
		free(str);
		fclose(fp);
		if(remove(tempFile)!=0)
			std::cerr << "Cant delete file:" << tempFile << std::endl;	
	}
	
	~SemWebDescriptionDumper()
	{
		delete _pool;
	}	
};

int processFile(const std::string & waveFile, const std::string & suffix)
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
	pMod = PyImport_ImportModule("SemWebCrawler"); //jun: relative path!!!
	if(!pMod)
	{
		std::cerr << "Cant open python file!" << std::endl;
		std::cerr << "Please check whether the path of the .py scripts is in the system path." << std::endl;
		return -1;
	}
	pDict = PyModule_GetDict(pMod);
	if(!pDict)
	{
		std::cerr<< "Cant find dictionary!"<< std::endl;
		return -1;
	}
	
	//create 1 parameter
	pParm = PyTuple_New(1);
	pClass = PyDict_GetItemString(pDict, "SemWebCrawler");
	PyTuple_SetItem(pParm, 0, Py_BuildValue("s",waveFile.c_str()));
	pInstance = PyInstance_New(pClass, pParm, NULL);   
	pRetVal=PyObject_CallMethod(pInstance, "spider", NULL);
	Py_DECREF(pMod);
	SemWebDescriptionDumper dumper;
	dumper.doIt();
	std::ofstream outputPool((waveFile+suffix).c_str());
	CLAM::XMLStorage::Dump(dumper.getPool(), "Description", outputPool);
	return 0;
}



int main(int argc, char* argv[])			// access command line arguments
{
	Py_Initialize();
	if(!Py_IsInitialized())
		return -1;
	PyRun_SimpleString("import sys\n");
	PyRun_SimpleString("sys.path.append('C:\\Jun\\devel\\Annotator\\src\\SemWebExtractor')"); //Alert: absolute path!!
	
	std::cout << copyright << std::endl;
	std::cout<<"Connecting to Linking Open Data Servers, Please wait.................."<<std::endl;
	std::list<std::string> songs;
	std::string suffix = ".pool";
	std::string schemaLocation = "";
	if (argc==1) 
	{
		std::cerr << usage << std::endl;
		return -1;
	}

	bool hasSchemaOption = false;
	bool isSchema = false;
	bool isSuffix = false;
	bool isConfiguration = false;
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
		else if (isConfiguration)
		{
			// TODO: Take the configuration file
			isConfiguration=false;
		}
		else if (parameter=="-f") isSuffix = true;
		else if (parameter=="-s") isSchema = hasSchemaOption = true;
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
		int error = processFile(*it, suffix);
		if (error) return error;
	}

	Py_Finalize();
	std::cout << "\n\nWeb-based Extraction is Done!" << std::endl;
	return 0;
}


