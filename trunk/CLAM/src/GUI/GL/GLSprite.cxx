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

#include "GLSprite.hxx"
using namespace CLAMGUI;

void GLSprite::MakeVertexData( int which )
{
	int stride = 6; // 2 floats for the Texture coords, 1 for the color and 3 floats for the vertex coords
	int offset = which*stride;

	InterleavedArray[ offset ] = TexCoords[which*2];
	InterleavedArray[ offset + 1 ] = TexCoords[which*2 + 1];
	InterleavedArray[ offset + 2 ] = Colors[which];
	InterleavedArray[ offset + 3 ] = VertexCoords[which*3];
	InterleavedArray[ offset + 4 ] = VertexCoords[which*3 + 1];
	InterleavedArray[ offset + 5 ] = VertexCoords[which*3 + 2];
}

void GLSprite::Init()
{
	// 0 is the bottom left corner
	// 1 is the bottom right corner
	// 2 is the top right corner
	// 3 is the top left corner
	mColors[0].SetData(255, 255, 255, 255);
	mColors[1].SetData(255, 255, 255, 255);
	mColors[2].SetData(255, 255, 255, 255);
	mColors[3].SetData(255, 255, 255, 255);

	// Building the Colors
	Colors[0] = mColors[0].StuffIntoFloat();
	Colors[1] = mColors[1].StuffIntoFloat();
	Colors[2] = mColors[2].StuffIntoFloat();
	Colors[3] = mColors[3].StuffIntoFloat();

	// Building the Vertex Array

	VertexCoords[0] = 0.0f; VertexCoords[1] = 0.0f; VertexCoords[2] = 0.0f;
	VertexCoords[3] = mWidth; VertexCoords[4] = 0.0f; VertexCoords[5] = 0.0f;
	VertexCoords[6] = mWidth; VertexCoords[7] = mHeigth; VertexCoords[8] = 0.0f;
	VertexCoords[9] = 0.0f; VertexCoords[10] = mHeigth; VertexCoords[11] = 0.0f;

	// Building the Texture Coordinates Array

	TexCoords[0] = 0.0f; TexCoords[1] = 1.0f;
	TexCoords[2] = 0.0f; TexCoords[3] = 0.0f;
	TexCoords[4] = 1.0f; TexCoords[5] = 0.0f;
	TexCoords[6] = 1.0f; TexCoords[7] = 1.0f;

	MakeVertexData( 0 );
	MakeVertexData( 1 );
	MakeVertexData( 2 );
	MakeVertexData( 3 );
}

void GLSprite::ImmediateDisplaySprite( GLenum TexturingMode ) const
{
	glPushMatrix();

	glTranslatef(mXPos, mYPos, 0);

	// Actual Drawing Code
	glBindTexture(TexturingMode, *mTextureID);
	glBegin(GL_QUADS);

	glColor3ub(255,255,255);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f( 0,0,0);

	glColor3ub(255,255,255);
	glTexCoord2f(0.0f, 0.0f);
	// bottom-right corner
	glVertex3f(mWidth, 0, 0);

	glColor3ub(255,255,255);
	glTexCoord2f(1.0f, 0.0f);
	// top-right corner
	glVertex3f( mWidth, mHeigth, 0);

	glColor3ub(255, 255, 255); // 255 indicates maximum opacity
	glTexCoord2f(1.0f, 1.0f);  // top-left corner
	glVertex3f( 0, mHeigth, 0);

	glEnd();


	glPopMatrix();

}

void GLSprite::ArrayDisplaySprite( GLenum TexturingMode ) const
{
	static GLubyte quadidxs[] = {0, 1, 2, 3};
	glPushMatrix();

	glTranslatef(mXPos, mYPos, 0);


	glInterleavedArrays(GL_T2F_C4UB_V3F,0, InterleavedArray );
	glBindTexture(TexturingMode, *mTextureID);
	glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, quadidxs );

	glFlush();

	glPopMatrix();
}
