/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : coding@reduz.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//
//
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "QtEnvelopePointEditor.hxx"
#include <qpainter.h>
#include <qcursor.h>
#include <math.h>

namespace CLAM 
{

static const float MIN_ZOOM=0.2;
static const float ZOOM_ADJUST_MARGIN_MIN=0; //percent of screen
static const float ZOOM_ADJUST_MARGIN_MAX=0; //percent of screen

bool QtEnvelopePointEditor::adjust_zoom_to_window() {

	float cur_width=width();
	float env_len=envelope->get_node_offset(envelope->get_node_count()-1);

	float adjust_margin_min=ZOOM_ADJUST_MARGIN_MIN*cur_width/100.0;
	float adjust_margin_max=ZOOM_ADJUST_MARGIN_MAX*cur_width/100.0;

	bool changed=false;

	float old_zoom=mZoom;

        if ( (env_len/mZoom) < (cur_width-adjust_margin_min) ) {

        	mZoom=env_len/(cur_width-adjust_margin_max);
	}

        if ( (env_len/mZoom) > (cur_width-adjust_margin_max) ) {

        	mZoom=env_len/(cur_width-adjust_margin_min);
	}


	if (mZoom<MIN_ZOOM) {
		mZoom=MIN_ZOOM;
	}

	if (mZoom!=old_zoom)
		changed=true;

	return changed;

}

void QtEnvelopePointEditor::move_mouse_to_selected_node() {

//	int cur_width=width();
	int cur_height=height()-mFontHeightSpacing;
//	int real_height=height();

	if (cur_height<=0)
		return;

        float env_normalize=-envelope->get_minY_value();
	float env_total=envelope->get_maxY_value()-envelope->get_minY_value();
	if (env_total==0)
		return; //canut be draw, broken envelope
	int center=int(env_normalize*(float)cur_height/env_total);
	center=cur_height-center;


	float node_x=(float)envelope->get_node_offset(mGrab.node);
	float node_y=(float)envelope->get_node_height(mGrab.node);

	node_x/=mZoom;
	node_y+=env_normalize;
	node_y=node_y*(float)cur_height/env_total;
	node_y=cur_height-node_y;

	QCursor::setPos(mapToGlobal(QPoint((int)node_x,(int)node_y)));
}


bool QtEnvelopePointEditor::event ( QEvent * e ) {

	if (envelope==NULL) return true;

	int cur_height=height()-mFontHeightSpacing;

	if (cur_height<=0)
		return true;


        float env_normalize=-envelope->get_minY_value();
	float env_total=envelope->get_maxY_value()-envelope->get_minY_value();
	if (env_total==0)
		return QWidget::event( e ); //canut be draw, broken envelope
	int center=int(env_normalize*(float)cur_height/env_total);
	center=cur_height-center;


	if (QEvent::MouseMove == e->type()) {
		QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(e);

		if (mGrab.moving_point) {

			float gx=mouseEvent->x(),gy=mouseEvent->y();
			gx*=mZoom;

			gy=float(cur_height-gy)*env_total/(float)cur_height;
			gy-=env_normalize;

			if ( ((int)gx!=envelope->get_node_offset(mGrab.node)) || ((int)gy!=envelope->get_node_height(mGrab.node)) ) {

				//avoid pointless overdraw
				// in CLAM this is a scoped_lock
//				if (envelope_lock) envelope_lock->Create();
				envelope->set_node_offset(mGrab.node,(int)gx,(int)gy);
//				if (envelope_lock) envelope_lock->release();
				if (adjust_zoom_to_window())
					move_mouse_to_selected_node();
				update();
			}
			//must setup new coordinates
		}
	}

	if (QEvent::MouseButtonRelease == e->type()) {
		QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(e);

		if (mGrab.moving_point && (mouseEvent->button()==LeftButton)) {

			mGrab.moving_point=false;
			releaseMouse();
			update();
		}
	}

	if (QEvent::MouseButtonPress == e->type()) {

		QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(e);

		float gx=mouseEvent->x(),gy=mouseEvent->y();

		// check if we grabbed anything.. distance in pixels
		int closest_node=-1;
		float closest_node_dist=1000;
		for (int i=0;i<envelope->get_node_count();i++) {


			float node_x,node_y;

			node_x=(float)envelope->get_node_offset(i);
			node_y=(float)envelope->get_node_height(i);

			node_x/=mZoom;
			node_y+=env_normalize;
			node_y=node_y*(float)cur_height/env_total;
			node_y=cur_height-node_y;

			float distance=sqrt(pow(node_x-gx,2)+pow(node_y-gy,2));

			if ( (distance<closest_node_dist) && (distance<(float)MIN_DRAG_DISTANCE)) {

				closest_node_dist=distance;
				closest_node=i;
			}

		}

		switch (mouseEvent->button()) {

			case LeftButton: {

				// check the fish
				if (closest_node!=-1) {

					//convert to local coords

					mGrab.moving_point=true;
					mGrab.node=closest_node;
					mGrab.x=envelope->get_node_offset(mGrab.node);
					mGrab.y=envelope->get_node_height(mGrab.node);
					grabMouse();
					move_mouse_to_selected_node();
				} else { //couldnt find close node, adding the point!


					gx*=mZoom; // to env coordinates
					gy=(cur_height-gy)*env_total/(float)cur_height;
					gy-=env_normalize;


//					if (envelope_lock) envelope_lock->mGrab();
                                	int which=envelope->add_node_at_offset((int)gx,(int)gy);
//					if (envelope_lock) envelope_lock->release();

					if (which>=0) {
						mGrab.moving_point=true;

						mGrab.node=which;
						mGrab.x=(int)gx;
						mGrab.y=(int)gy;
						grabMouse();
						move_mouse_to_selected_node();
						update();
						 envelopeChanged();

					}
				}

			} break;
			case RightButton: {

        			if (closest_node!=-1) {

//					if (envelope_lock) envelope_lock->mGrab();
        				envelope->delete_node(closest_node);
//					if (envelope_lock) envelope_lock->release();
					update();
					 envelopeChanged();
					adjust_zoom_to_window();
				}
			} break;
			default: {}
		}


	}

	QWidget::event(e);
	return true;
}


void QtEnvelopePointEditor::set_envelope(QtEnvelope *p_envelope) {

	envelope=p_envelope;
	adjust_zoom_to_window();
	mRecalcPending=true;
	update();
}


void QtEnvelopePointEditor::paint_envelope_points(QPainter &painter) {

	int cur_height=height()-mFontHeightSpacing;
	int cur_width=width();
	int real_height=height();


	if (cur_height<=0)
		return;

	float env_normalize=-envelope->get_minY_value();
	float env_total=envelope->get_maxY_value()-envelope->get_minY_value();

	if (envelope->is_loop_enabled()) 
	{

		float node_begin,node_end;

		node_begin=envelope->get_node_offset( envelope->get_loop_begin() )/mZoom;
		node_end=envelope->get_node_offset( envelope->get_loop_end() )/mZoom;

		painter.setPen(QColor(170,170,170));
		//XAMAT: painter.setPen(QColor(0,200,0));

        	painter.drawLine((int)node_begin,0,(int)node_begin,cur_height);
        	painter.drawLine((int)node_end,0,(int)node_end,cur_height);
	}

	if (envelope->is_sustain_loop_enabled()) 
	{

		float node_begin,node_end;

		node_begin=envelope->get_node_offset( envelope->get_sustain_loop_begin() )/mZoom;
		node_end=envelope->get_node_offset( envelope->get_sustain_loop_end() )/mZoom;

		 painter.setPen(QColor(220,220,220));
		//XAMAT:painter.setPen(QColor(0,200,0));

        	painter.drawLine((int)node_begin,0,(int)node_begin,cur_height);
        	painter.drawLine((int)node_end,0,(int)node_end,cur_height);
	}
	//xamat
	painter.fillRect(0,cur_height,cur_width,real_height-cur_height,QColor(0,0,0));

	painter.setPen(QColor(200,200,200));
	
	for (int i=0;i<envelope->get_node_count();i++) {


		float node_x,node_y;

		node_x=(float)envelope->get_node_offset(i)/mZoom;

		node_y=(float)envelope->get_node_height(i);
		node_y+=env_normalize;
		node_y=node_y*(float)cur_height/env_total;
		node_y=cur_height-node_y;

		painter.drawRect((int)node_x-2,(int)node_y-2,6,6);
	}

	painter.setFont(mDisplayFont);
	const QFontMetrics &display_font_metrics=painter.fontMetrics();

	char buf[200];

	painter.setPen(QColor(200, 200, 200));

	/* This code calculates the increment for displaying the numbers, so they dont overlap */
	float avg_string_width=(int)display_font_metrics.width("xxxxx");

	float increment=1.0;
	int step=0;

	while ((increment/mZoom)<avg_string_width) {


		if (step % 2)
			increment*=2;
		else
			increment*=5;
		step++;

	}
        for (float i=0.0f;i<(cur_width*mZoom);i+=increment) {

		int real_screen_offset=int((float)i/mZoom);

		sprintf(buf,"%i",(int)((float)i*mTimeFactor));
		if (i>0.0f)
			painter.drawText(real_screen_offset-display_font_metrics.width(buf)/2,cur_height+display_font_metrics.height(),buf);
	}

	if (mGrab.moving_point) {

		sprintf(buf,"mZoom %i%%, %i nodes. Moving node %i, pos: %i,%i",int(mZoom*100.0),envelope->get_node_count(),mGrab.node,envelope->get_node_offset(mGrab.node),envelope->get_node_height(mGrab.node));
	} else {

		sprintf(buf,"mZoom %i%%, %i nodes. ",int(mZoom*100.0),envelope->get_node_count());
	}

	//XAMAT: we don't need this text:
	//painter.drawText(2,cur_height+3+display_font_metrics.height()*2,buf);

}

void QtEnvelopePointEditor::draw_envelope_pos(int p_x,QPainter &painter) {

//	int cur_width=width();
	int cur_height=height()-mFontHeightSpacing-1;
//	int real_height=height();

	if (cur_height<=0)
		return;

        float env_normalize=-envelope->get_minY_value();
	float env_total=envelope->get_maxY_value()-envelope->get_minY_value();
	if (env_total==0)
		return; //canut be draw, broken envelope
	int center=int(env_normalize*(float)cur_height/env_total);
	center=cur_height-center;

	if (mPositionCache[p_x]) {

	  painter.setPen(QColor(255,255,255));
	  
	} else {

	  painter.setPen(QColor(0,0,0));
	}

	painter.drawLine(p_x,0,p_x,cur_height);


	int real_x_offset=int(p_x*mZoom);
	int prev_real_x_offset=int((p_x-1)*mZoom);

	int line_size=1;


	if (real_x_offset!=prev_real_x_offset) {

		if ((real_x_offset % 5)==0) {

			line_size=3;
		}

		if ((real_x_offset % 10)==0) {

			line_size=5;
		}

		if ((real_x_offset % 50)==0) {

			line_size=7;
		}

		if ((real_x_offset % 100)==0) {

			line_size=9;
		}

		if ((real_x_offset % 500)==0) {

			line_size=11;
		}
	}

	painter.setPen(QColor(200,200,255));
	//XAMAT: painter.setPen(QColor(0,255,0));
	int end=center+(line_size/2);

	if (end>=(cur_height))
		end=cur_height;

	painter.drawLine(p_x,center-(line_size/2),p_x,end);


	float val=envelope->get_height_at_pos((float)p_x*mZoom);

	if (p_x*mZoom>envelope->get_node_offset(envelope->get_node_count()-1))
		return; //dont draw what it's pointless to draw

	val+=env_normalize;
	val=val*(float)cur_height/env_total;
	val=cur_height-val;

	float prev_val=envelope->get_height_at_pos((float)(p_x-1)*mZoom);
	prev_val+=env_normalize;
	prev_val=prev_val*(float)cur_height/env_total;
	prev_val=cur_height-prev_val;


	//XAMAT-g painter.setPen(QColor(140,80,80));
	painter.setPen(QColor(0,255,0));
	#define MIN(m_a,m_b) ( ((m_a)<(m_b))?(m_a):(m_b))
	#define MAX(m_a,m_b) ( ((m_a)>(m_b))?(m_a):(m_b))
	painter.drawLine(p_x,(int)MIN(prev_val,val)-1,p_x,(int)MAX(prev_val,val)+1); //must make it better


	//XAMAT-g painter.setPen(QColor(255,150,150));
	painter.setPen(QColor(0,255,0));
	painter.drawLine(p_x,(int)prev_val,p_x,(int)val); //must make it better

}

void QtEnvelopePointEditor::paintEvent( QPaintEvent * p_event ) {


	if (envelope==NULL) return;

	if (mRecalcPending) {
		adjust_zoom_to_window();
		mRecalcPending=false;
	}

	if ( (int)mPositionCache.size()!=width() ){

		mPositionCache.resize(width());
		mOldPositionCache.resize(width());
	}

//	float prev;
	QPainter painter;
//	painter.begin(backing_store, this );
	painter.begin(this );


	painter.setPen(QColor(0,0,200));



	for (int i=0;i<width();i++) {

		draw_envelope_pos(i,painter);
	}

	paint_envelope_points(painter);


}



void QtEnvelopePointEditor::update_position_list(const std::vector<int>& p_positions) {

	if ( (int)mPositionCache.size()!=width() ){

		mPositionCache.resize(width());
		mOldPositionCache.resize(width());
	}

	if (!isVisible())
		return; //do nothing if not visible


	for (int i=0;i<(int)mPositionCache.size();i++) {

		mPositionCache[i]=false;
	}

	for (int i=0;i<(int)p_positions.size();i++) {

		float pos=p_positions[i];
		pos/=mZoom;
                if ((int)pos>=width())
			continue;

		mPositionCache[(int)pos]=true;
	}

	QPainter painter;
	painter.begin(this );

	for (int i=0;i<width();i++) {

		if (mPositionCache[i] || mOldPositionCache[i]) {

			draw_envelope_pos(i,painter);
		}
	}

	paint_envelope_points(painter);
	mOldPositionCache=mPositionCache;


}



QtEnvelopePointEditor::QtEnvelopePointEditor(QWidget *p_parent) : QWidget(p_parent) {

  //xamat
	setBackgroundMode (NoBackground);
	
	envelope=NULL;
	mZoom=0.25f;
	mTimeFactor = 1;

	mDisplayFont.setFamily("fixed");
	mDisplayFont.setPointSize(10);
	mDisplayFont.setBold(true);
	mDisplayFont.setStyleHint(QFont::Courier,QFont::NoAntialias);


	QFontMetrics display_font_metrics(mDisplayFont);
	mFontHeightSpacing=display_font_metrics.height()*2+6;

	mGrab.moving_point=false;
	mGrab.node=-1;
	mGrab.x=0;
	mGrab.y=0;
	mRecalcPending=true;
	//envelope_lock=NULL;
}

QtEnvelopePointEditor::~QtEnvelopePointEditor()
{
}

} // namespace CLAM

