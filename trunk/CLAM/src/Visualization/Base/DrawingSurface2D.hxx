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

/** \file DrawingSurface2D.hxx
 */

#ifndef __DRAWINGSURFACE2D__
#define __DRAWINGSURFACE2D__

#include "Slotv1.hxx"
#include "Slotv2.hxx"
#include "Slotv4.hxx"
#include "BoundingBoxes.hxx"

/**  \namespace CLAMVM 
 *   \brief     The Visualization Module components namespace.
 *
 *   This namespace should contain all the generic components that conform
 *   CLAM Visualization Module.
 */
namespace CLAMVM
{

	using SigSlot::Slotv1;
	using SigSlot::Slotv2;
	using SigSlot::Slotv4;

/** \class DrawingSurface2D
 *  \brief Interface for 2D drawing canvases.
 *
 *  This abstract class provides the interface and some basic common functionality
 *  for modeling the concept of a 2D canvas for drawing functions, providing a projection
 *  from WCS ( the world coordinate system ) to SCS( screen coordinate system ).
 *  The metaphor consists in identifying a canvas to a certain rectangle in WCS defined
 *  by the left, right, bottom, top values; as well as the underlying framebuffer portion
 *  where you are going to draw on.
 */
	class DrawingSurface2D
	{
	public:
		/** \brief  Default constructor
		 *
		 *  Initializes the canvas bounds.
		 */
		DrawingSurface2D();

		/** \brief Destructor
		 *
		 */
		virtual ~DrawingSurface2D();

		/** \brief Enables indexed mode support 
		 *
		 *  "Indexed Mode" is a commonly used technique to reduce the memory necessary for
		 *  representing an image, by mapping, typically, an integer value into (R,G,B) triplet. This
		 *  integer value receives the name of "index". So instead of needing 3 numbers for representing a pixel
		 *  color, you use just one.
		 *  The table containing the mapping is sometimes referred to as "colormap", "ramp" or "palette". Translation
		 *  from indexes to actual (R,G,B) colors is performed by the Graphics Device Driver. Control
		 *  over this translation is usually offered by the OS ( thus being a OS-dependant interface ).
		 *  Nowadays this technique is seldom used, but sometimes can come very in handy.
		 */
		virtual void EnableIndexedMode() = 0;
		
		/** \brief Disables Indexed Mode support
		 * 
		 *  Reverts the surface back into explicit color representation
		 */
		virtual void DisableIndexedMode() = 0;
		
		/** \brief Queries the surface about Indexed mode
		 *
		 *  This method returns a boolean telling if the surface is configured for
		 *  for drawing in indexed mode.
		 *
		 *  \return True if indexed mode drawing is enabled. False otherwise.
		 */
		virtual bool IsIndexedModeEnabled() const = 0;
		
		/** \brief Enables double buffering for this surface
		 *
		 *  "Double buffering" is a common Computer Graphics for achieving smooth animations while drawing on the
		 *  screen. In order to avoid images to flicker while refreshing the screen, this technique involves the usage
		 *  of two buffers: a "front buffer" and a "back buffer". While the "front buffer" is being displayed by the
		 *  Graphics Device, one draws the next frame into the back-buffer. Good graphics APIs provide a clear, simple
		 *  interface for using this technique - which can be critical when displaying data in real-time.
		 */
		virtual void EnableDoubleBuffering() = 0;
		
		/** \brief Disables doublebuffering
		 *  
		 *  This function disables double buffering for this surface
		 */
		virtual void DisableDoubleBuffering() = 0;

		/** \brief Queries the surface about double-buffering status
		 *
		 *  This method returns a boolean whose value depends on the double-buffering status
		 *  for this surface.
		 *
		 *  \return True if double buffering is enabled. False otherwise.
		 */
		virtual bool IsDoubleBufferingEnabled() const = 0;

		/** \brief Tells if the Canvas has to recalculate its projection matrix
		 *
		 *  Whenever one of the bounds of the canvas is changed it is needed to recalculate
		 *  the projection matrix that maps WCS coords into SCS coords. This Accessor is provided
		 *  so concrete canvases can avoid unnecessary calculations of this matrix
		 *
		 *  \returns A boolean telling if the projection matrix must be recalculated
		 */
		bool MustReproject() const
		{
			return mMustReproject;
		}

		/**  \brief  Queries the canvas bounding box.
		 *   
		 *   Returns ( by parameter ) the current bounding box for the canvas.
		 *   \see    DataBoundBox
		 *   \param  bbox        The canvas current bounding box
		 */
		void QueryDataBoundBox( DataBoundBox& bbox);

		/**
		 *  \brief  Queries the canvas screen coordinates
		 *
		 *  Returns ( by parameter ) the current screen space bounding box for the canvas
		 *  \see    ScreenBoundBox
		 *  \param  sbbox          The current canvans screen space bounding box
		 */
		virtual void QueryScreenBoundBox( ScreenBoundBox& sbbox ) = 0;
		
		void SetTooltipText( const char* text ) {
			mTooltipText = text;
		}

		const char* TooltipText() const {
			return mTooltipText;
		}

		virtual void RedrawOverlay() = 0;

		/** \brief  Sets the World Space Coordinates Bounding box
		 *  
		 *  This slot takes four parameters ( right, left, top, bottom ) which defines
		 *  the coordinates of the corners of the WCS bounding box ( the CS where you
		 *  have your function/objects defined ). These parameters determine the form
		 *  the projection matrix ( that maps your values in WCS into coords in SCS ).
		 *
		 *  You can also understand this World Space as the view frustrum, i.e. a description
		 *  of the space visible from the point of view of the camera ( assuming an orthographic
		 *  projection, of course ).
		 *
		 *  \param right   The highest x value for the bounding box
		 *  \param left    The lowest x value for the bounding box
		 *  \param top     The highest y value for the bounding box
		 *  \param bottom  The lowest y value for the bounding box
		 */
		Slotv4< double, double, double, double>  SetWorldSpace;

		/** \brief Sets the lowest x bounding box value
		 *
		 *  This slot takes as parameter the new left limit ( lowest x value ) for the
		 *  canvas bounding box.
		 *
		 *  \param left  The lowest x value
		 *  \sa    SetWorldSpace, SetRight, SetTop, SetBottom
		 */
		Slotv1< double >                         SetLeft;

		/** \brief Sets the highest x bounding box value
		 *
		 *  This slot takes as parameter the new right limit ( highest x value ) for the
		 *  canvas bounding box.
		 *
		 *  \param right  The highest x value
		 *  \sa    SetWorldSpace, SetLeft, SetTop, SetBottom
		 */
		Slotv1< double >                         SetRight;

		/** \brief Sets the highest y bounding box value
		 *
		 *  This slot takes as parameter the new upper limit ( highest y value ) for the
		 *  canvas bounding box.
		 *
		 *  \param top  The highest y value
		 *  \sa    SetWorldSpace, SetLeft, SetRight, SetBottom
		 */
		Slotv1< double >                         SetTop;

		/** \brief Sets the lowest y bounding box value
		 *
		 *  This slot takes as parameter the new upper limit ( highest y value ) for the
		 *  canvas bounding box.
		 *
		 *  \param top  The lowest y value
		 *  \sa    SetWorldSpace, SetLeft, SetRight, SetBottom
		 */
		Slotv1< double >                         SetBottom;

		/** \brief  Adjust bounding box width
		 *
		 *  This "adjustment" consists in translating and scaling the bounding box by two
		 *  amounts the offset ( the translation component ) and the scale ( the scaling component ).
		 *  These amounts are given in a percentual way: the pair ( 0.25, 1.10 ) stands for "translate
		 *  the bounding box in respect the x-axis a 25 percent" and "scale the bounding box in respect
		 *  the x-axis a 110 percent".
		 *
		 *  \param offset The amount the bbox has to be translated
		 *  \param scale  The amount the bbox has to be scaled
		 */
		Slotv2< double, double>                  AdjustXAxis;

		/** \brief  Adjust bounding box height
		 *
		 *  This "adjustment" consists in translating and scaling the bounding box by two
		 *  amounts the offset ( the translation component ) and the scale ( the scaling component ).
		 *  These amounts are given in a percentual way: the pair ( 0.25, 1.10 ) stands for "translate
		 *  the bounding box in respect the y-axis a 25 percent" and "scale the bounding box in respect
		 *  the y-axis a 110 percent".
		 *
		 *  \param offset The amount the bbox has to be translated
		 *  \param scale  The amount the bbox has to be scaled
		 */
		Slotv2< double, double>                  AdjustYAxis;

	protected:
		void _SetWorldSpace( double xmax, double xmin, double ymax, double ymin );
		void _AdjustXAxis( double offset, double scale );
		void _AdjustYAxis( double offset, double scale );
		void _SetLeft( double left );
		void _SetRight( double right );
		void _SetTop( double top );
		void _SetBottom( double bottom );

		/** \brief  Forces Projection matrix recalculation
		 *
		 *  This method forces projection matrix recalculation during the next
		 *  canvas refreshing. This is useful to refresh position and zoom factor
		 *  when changing it with sliders.
		 */
		void DamageProjection()
		{
			mMustReproject = true;
		}

		/** \brief Applies WCS
		 *  
		 *  This method calculates the ortographic ( or other ) perspective projection matrix.
		 */
		virtual void WCSProject() = 0;
		/** \brief Applies SCS
		 *
		 *  This method calculates the window-viewport projection matrix.
		 */
		virtual void WindowProject() = 0;
		/** \brief Draws the contents of the canvas 
		 *
		 *  Executes the different algorithms used for representing the data.
		 */
		virtual void DrawContents()=0;
		/** \brief Recalculate projection and redraw
		 *
		 * Redraws the canvas contents and updates the perspective and window-viewport matrixs
		 */
		virtual void Refresh() = 0;

		
	private:
		bool        mMustReproject;
		double      mxdist, mydist;
		const char* mTooltipText;

	protected:
		
		double mxmax, mxmin, mymax, mymin;
		double mcxmax, mcxmin, mcymax, mcymin;
	};

}

#endif // DrawingSurface2D.hxx
