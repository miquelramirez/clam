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

#include "Port.hxx"

namespace CLAM {


	// Data visitor
	//\todo change exceptions for asserts
	void DataVisitor::ErrMsg(std::string &msg,const std::string &data_class)
	{
		msg=  "No Visit method implemented in visitor ";
		msg+= ClassName() + " for the ";
		msg+= data_class + " data class\n";
	}

	void DataVisitor::Visit(ProcessingData& data)
	{
		throw Err("DataVisitor::Visit(): Visitor not implemented for the data class\n");
	}
	void DataVisitor::Visit(Spectrum& data)
	{
		std::string msg;
		ErrMsg(msg,"Spectrum");
		throw(Err(msg.c_str()));
	}
	void DataVisitor::Visit(Audio& data)
	{
		std::string msg;
		ErrMsg(msg,"Audio");
		throw(Err(msg.c_str()));
	}
	void DataVisitor::Visit(Peak& data)
	{
		std::string msg;
		ErrMsg(msg,"Peak");
		throw(Err(msg.c_str()));
	}
	void DataVisitor::Visit(Envelope& data)
	{
		std::string msg;
		ErrMsg(msg,"Envelope");
		throw(Err(msg.c_str()));
	}



} // namespace
