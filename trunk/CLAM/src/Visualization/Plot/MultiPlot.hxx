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

#ifndef __MULTIPLOT__
#define __MULTIPLOT__

#include "DataTypes.hxx"
#include "Array.hxx"
#include "BPF.hxx"
#include "Color.hxx"
#include <string>
#include <map>
#include "GenericPlot.hxx"

namespace CLAMVM
{
	class SequenceRenderingManager;

	class MultiPlot : public GenericPlot
	{
	public:
		MultiPlot( );
		MultiPlot( std::string name );
		~MultiPlot();
		
		void SetColor( std::string key, Color c );
		void AddData( std::string key, const CLAM::Array<CLAM::TData>& array, double xmin, double xmax );
		void AddData( std::string key, const CLAM::BPF&, int samples = 100 );
		void RemoveData( std::string key );
		void RemoveAllData();

	protected:

		virtual void  CreateWidget();		
		bool          ExistsRenderer( const std::string& key );
		void          CreateNewRenderer( const std::string& key );
		void          UpdateWidgetRenderers();
		double        MinimumXOfAll();
		double        MaximumXOfAll();

	protected:		
		typedef std::map< std::string, SequenceRenderingManager* > RendererTable;

		RendererTable                  mRenderers;
		
	};


}



#endif // Multiplot.hxx
