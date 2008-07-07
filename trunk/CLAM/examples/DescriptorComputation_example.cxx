/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Segment.hxx"
#include "XMLStorage.hxx"
#include "SegmentDescriptors.hxx"
#include "DescriptorComputation.hxx"
#include <QtGui/QFileDialog>
#include <QtGui/QApplication>

int main( int argc, char** argv )
{
	QApplication app(argc, argv);

	CLAM::Segment segment;

	QString file = QFileDialog::getOpenFileName(
		0,
		"Please select an XML analysis file",
		QString::null,
		"*.xml");

	if (file.isEmpty())
	{
		std::cerr << "No analuysis file was selected!" << std::endl;
		exit(0);
	}


	std::cout<<"Loading Analysis File. Please Wait."<<"\n";
	CLAM::XMLStorage::Restore(segment, file.toStdString().c_str());
	std::cout<<"Analysis File Loaded Successfully"<<"\n";

	std::cout<<"Computing Descriptors. Please Wait."<<"\n";

	CLAM::SegmentDescriptors segmentDescriptors;
	segmentDescriptors.AddAll();
	segmentDescriptors.UpdateData();

	CLAM::SpectralDescriptors specProto;
	specProto.AddAll();
	specProto.UpdateData();

	CLAM::SpectralPeakDescriptors peakProto;
	peakProto.AddAll();
	peakProto.UpdateData();

	CLAM::FrameDescriptors frameProto;
	frameProto.AddAll();
	frameProto.UpdateData();
		
	frameProto.GetSpectralPeakD().SetPrototype(peakProto);
	frameProto.GetResidualSpecD().SetPrototype(specProto);
		
	segmentDescriptors.SetFramePrototype(frameProto,segment.GetnFrames());
	segmentDescriptors.SetpSegment(&segment);
		
	//we can directly call the compute operation on the descriptor
	//segmentDescriptors.Compute();
		
	//or use an intermediate DescriptorComputation processing
	CLAM::DescriptorComputation processing;
	processing.Do(segmentDescriptors);
		
	std::cout<<"Descriptors Computed Successfully"<<"\n";
		
	QString outFilename = QFileDialog::getOpenFileName(
		0,
		"Please enter name of where you want your output descriptors to be stored",
		QString::null,
		"*.xml");

	if (file.isEmpty())
	{
		std::cerr << "No file was specified defaulting to: 'Descriptors.xml'" << std::endl;
		outFilename = "Descriptors.xml";
	}
	

	std::cout<<"Storing Results into xml file. Please Wait."<<"\n";
	CLAM::XMLStorage::Dump(segmentDescriptors,"segmentDescriptors",outFilename.toStdString().c_str());

	std::cout<<"Program finished Successfully"<<"\n";
	return 0;
}


