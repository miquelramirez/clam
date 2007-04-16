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

#include "MyProcessingWithControlArray.hxx"

namespace CLAM 
{

		//InControlTmplArray(int size, const std::string &name, Processing* parent=0,
		//TPtrMemberFuncId f, const bool publish);
MyProcessingWithControlArray::MyProcessingWithControlArray() :
	mSubPOsTransformation( NVoices ),
	mIn(12, "my_name", this, &MyProcessingWithControlArray::CallbackId),
	mIn2("my_name", this, &MyProcessingWithControlArray::Callback),
	mInArray(10, "array_control_name", this, &MyProcessingWithControlArray::CallbackId),
	mVoiceControl(NVoices, "VoiceControl", this, &MyProcessingWithControlArray::VoiceControlCallbackId)
{
	for(int i=0; i<NVoices; i++)
		mSubPOsTransformation.at(i) = new TransformationDummy;
}

MyProcessingWithControlArray::~MyProcessingWithControlArray()
{
	for(int i=0; i<NVoices; i++)
		delete mSubPOsTransformation.at(i);
}



int MyProcessingWithControlArray::CallbackId(int id, TControlData val) {
	std::cout << "MyProcessingWithControlArray::CallbackId(id: " << id 
		<< " val: " << val << " )" << std::endl;
	return 0; // TODO: depracate result
}

int MyProcessingWithControlArray::VoiceControlCallbackId(int id, TControlData val)
{
	CLAM_DEBUG_ASSERT(id < mSubPOsTransformation.size(), "id is larger than array size");
	std::cout << "VoiceControlCallbackId with Id: " << id << std::endl;
	mSubPOsTransformation.at(id)->voiceInControl.DoControl(val);
	return 0;
}

}//namespace

