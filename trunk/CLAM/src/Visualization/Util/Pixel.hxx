/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _PIXEL_
#define _PIXEL_

#include "Array.hxx"
#include <iosfwd>
using CLAM::Array;

namespace CLAMGUI
{
/**
 *  Utility class for representing a color. The class name will
 *  be changed soon to Color. Formally, it represents a vector
 *  in a color space. This class could be templatized in order
 *  to support floating point color component values as well as
 *  integers
 */
	class Pixel
	{
	private:
		unsigned char*  data;
		int bpp;

		inline void MakeData(unsigned char r, unsigned char g, unsigned char b)
		{
			data[0] = r;
			data[1] = g;
			data[2] = b;
		}

		inline void MakeData( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
		{
			data[0] = red;
			data[1] = green;
			data[2] = blue;
			data[3] = alpha;
		}

	public:

		/**
		 * Default Constructor
		 */

		Pixel()
			: data( NULL )
		{
			bpp = 0;
		}

		/**
		 *  Constructor
		 *  @param red The color R component value in a 0-255 range
		 *  @param green The color G component value in a 0-255 range
		 *  @param blue The color B component value in a 0-255 range
		 */
 
		Pixel(unsigned char red, unsigned char green, unsigned char blue, int depth) 
		  :  data( NULL ), bpp(depth)
		{
			data = new unsigned char[bpp];
			MakeData(red, green, blue);
		}

		Pixel(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) 
			: data( NULL ), bpp(4)
		{
			data = new unsigned char[bpp];
			MakeData(red, green, blue, alpha);
		}

		/**
		 *  Copy <EM>constructor</EM>
		 */
		Pixel ( const Pixel& p)
		{
			data = NULL;
			this->SetData(p.GetData(), p.GetBpp() );
		}
		/**
		 *  Copy <EM>operator</EM>
		 */
		inline Pixel& operator=(const Pixel& p)
		{
			data = NULL;

			this->SetData(p.GetData(), p.GetBpp() );

			return *this;
		}

		/**
		 *  This Accessor gives us a pointer to
		 *  the component bytes themselves
		 */
		inline unsigned char* GetData() const
		{
			return data;
		}

		/**
		 *  This allows to change the whole color
		 *  @param data2 The component array
		 *  @param bpp The desired bpp (color depth expressed as bytes-per-pixel)
		 */

		inline void SetData(unsigned char r, unsigned char g, unsigned char b)
		{
			bpp = 3;
			if (data)
			{
				delete [] data;
			}
			data = new unsigned char[bpp];

			MakeData(r,g,b);
		}

		inline void SetData(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			bpp = 4;
			if (data)
			{
				delete [] data;
			}
			data = new unsigned char[bpp];

			MakeData(r,g,b,a);
		}

		void SetData(unsigned char* data2, int bpp)
		{
			this->bpp = bpp;

			if (data!=NULL)
			{
				delete[] data;
			}

			data = new unsigned char[bpp];

			for (int k = 0; k < bpp ; k++ )
			{
				data[k] = data2[k];
			}
		}

		float StuffIntoFloat() const
		{
			float value	= 0.0f;

#ifdef CLAM_BIG_ENDIAN
			value = data[0] << 24 | data[1] << 16| data[2] << 8 | data[4];
#else
			value = float(data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24);
#endif

			return value;
		}

		/**
		 *  This Accessor gives us the current colordepth
		 *  for the color
		 *  @return The Colordepth
		 */
 
		int GetBpp() const { return bpp; }
	  
		/**
		 *  Destructor
		 */
		virtual ~Pixel()
		{
			if (data)
			{
				delete [] data;
				data =NULL;
			}

			//if (!data)
			//	std::cout << "Hmmm, it was a NULL" << std::endl;
		}
	};

	/**
	 * @todo: Not yet implemented
	 */
	std::istream& operator >> (std::istream& myStream, const Pixel& p);

	/**
	 * @todo: Not yet implemented
	 */
	std::ostream& operator << (std::ostream& myStream, const Pixel& p);

	/**
	 *  Array Specialization for representing a serie of colorspace vectors
	 *  thus becoming a color ramp
	 */
	typedef Array<Pixel>	TColorPath;

}

#endif//_PIXEL_

