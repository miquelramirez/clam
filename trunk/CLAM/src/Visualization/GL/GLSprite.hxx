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

#ifndef _GLSPRITE_
#define _GLSPRITE_

#include "CLAMGL.hxx"
#include "DataTypes.hxx"
#include <vector>
#include "Pixel.hxx"
using CLAM::TData;

namespace CLAMGUI
{
	/**
	 *	Utility class for drawing effectively (and efficiently) a
	 *  sprite (i.e. 2D pixmap) using OpenGL. The idea was taken (<EM>taken</EM>
	 *  not copied) from "Game Programming Gems" edited by Charles River Media, a
	 *  truly gem BTW
	 */
	class GLSprite
	{
	private:
		GLuint		mKludgeVar;
		TData		mHeigth;
		TData		mWidth;
		GLuint*		mTextureID;
		TData		mXPos;
		TData		mYPos;
		Pixel		mColors[4];

		GLfloat		TexCoords[8];
		GLfloat		VertexCoords[12];
		GLfloat		Colors[4];

		GLfloat		InterleavedArray[24];

		void MakeVertexData( int which );

		void Init();

	public:

		/**
		 *	Default constructor
		 */
		GLSprite() : mKludgeVar(0), mHeigth(0), mWidth(0), mTextureID(0), mXPos(0), mYPos(0)
		{
			Init();
		}
		/**
		 *	Constructor
		 *  @param w Width
		 *  @param h Heigth
		 */
		GLSprite(TData w, TData h):mKludgeVar(0), mHeigth(h), mWidth(w),  mTextureID(0),  mXPos(0), mYPos(0)
		{
			Init();
		}
		/**
		 *	Constructor
		 *  @param x X coord for the Sprite
		 *  @param y Y coord for the Sprite
		 *  @param w Width for the Sprite
		 *  @param h Heigth for the Sprite
		 *  Note that these coordinates are <EM>not</EM> viewport coordinates,
		 *  they are <EM>world</EM> coordinates
		 *  @todo x,y,w,h should be replaced by a Recti object
		 *  @see Recti
		 */
		GLSprite(TData x, TData y, TData w, TData h) : mKludgeVar(0), mHeigth(h), mWidth(w),  mTextureID(0), mXPos(x), mYPos(y)
		{
			Init();
		}
		/**
		 *	Destructor
		 */
		virtual ~GLSprite()
		{
		}
		/**
		 *	It draws the Sprite where you say.
		 *  @param x Desired x coord for the sprite
		 *  @param y Desired y coord for the sprite
		 */
		void SetPosition(TData x, TData y)
		{
			mXPos = x;
			mYPos = y;
		}

		void SetDims( float w, float h )
		{
			mWidth = w;
			mHeigth = h;
		}

		/**
		 *	Assigns to the Sprite an OpenGL Texture
		 *	@param texID OpenGL texture identifier
		 */
		void SetPixMap ( GLuint* texID) 
		{
			mTextureID=texID;
		}


		/** Returns the Texture Id associated to this sprite
		 *	@return The Texture Id
		 */

		GLuint* GetPixMap() const { return mTextureID; }

		/**
		 *	Checks the identifier assigned to this Sprite is valid or not
		 */
		bool HasValidPixMap () const { return (*mTextureID!=0); }

		/**
		 *	OpenGL drawing code for the Sprite
		 */
		void ImmediateDisplaySprite(GLenum TexturingMode) const;

		void ArrayDisplaySprite(GLenum TexturingMode) const;


		static void SetOpenGLState(GLenum TexturingMode) 
		{
			// glEnable(GL_BLEND);
			// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(TexturingMode);

			// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}

		static void UnSetOpenGLState(GLenum TexturingMode) 
		{
			glDisable( TexturingMode );
			//glDisable( GL_BLEND );
		}
	};

	/**
	 *  Specialization of std::vector for containing a sequence of Sprites.
	 *  It was not intended to become by any means to become a professional 
	 *  keyframed animation support structure, but it can be used as such
	 *  with some caution :)
	 */

	typedef std::vector<GLSprite*>	SpriteBuffer;
}

#endif // GLSprite.hxx

