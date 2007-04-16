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
/***************************************************************************
                          envelope.cpp  -  description
                             -------------------
    begin                : Sun Mar 25 2001
    copyright            : (C) 2001 by Juan Linietsky
    email                : reduz@anime.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "QtEnvelope.hxx"

namespace CLAM 
{

	QtEnvelope::QtEnvelope() {

		max_nodes=INFINITE_NODES;
		min_nodes=2;
		reset();
	}

	void QtEnvelope::reset() {


		on=false;
		loop_on=false;
		loop_begin_node=0;
		loop_end_node=0;
		sustain_loop_on=false;
		sustain_loop_begin_node=0;
		sustain_loop_end_node=0;
		fadeout=0;
		node.clear();
		loop_begin_node=0;
		loop_end_node=0;
		sustain_loop_begin_node=0;
		sustain_loop_end_node=0;
	}

	float QtEnvelope::get_height_at_pos(float p_pos) {

		if (node.size()<2) return 0;
		if (p_pos>node[node.size()-1].offset)
			return node[node.size()-1].value;

		float begin_x,begin_y;
		float end_x,end_y,xdif;
		unsigned count=0;
		int limit=-1;

		//todo - use a binary search

		while ((count<node.size()) && (limit==-1)) {

			if (node[count].offset>=p_pos) limit=count;
			count++;
		}

		if (p_pos==0) return node[0].value;

		if (limit==-1) return NO_POINT;

		begin_x=node[limit-1].offset;
		end_x=node[limit].offset;
		begin_y=node[limit-1].value;
		end_y=node[limit].value;

		xdif=end_x-begin_x;
		return begin_y+((p_pos-begin_x)*(end_y-begin_y))/(xdif?xdif:1);
	}

	void QtEnvelope::set_node_offset(int p_node, int p_x, int p_y) {

		if (p_node>=node.size()) return;

		if (p_node==0) {

			p_x=0;

		} else if (p_x<=node[p_node-1].offset) {

		   p_x=node[p_node-1].offset+1;

		} else if ((p_node<node.size()-1) && (p_x>=node[p_node+1].offset)) {
		  p_x=node[p_node+1].offset-1;
		}

		if (p_x>maxX_value) 
		  {
		    p_x=maxX_value;
		  }
		if (p_x<minX_value)
		  {
		    p_x=minX_value;
		  }

		if (p_y>maxY_value) p_y=maxY_value;
		if (p_y<minY_value) p_y=minY_value;

			node[p_node].offset=p_x;
			node[p_node].value=p_y;
		
	}

	int QtEnvelope::add_node_at_offset( int p_x, int p_y) {


		int i,new_node;

		if (node.size()>=max_nodes)
			return -1;

		// if this is assigning an existing node, let's quit.
		for (i=0;i<node.size();i++) if (p_x==node[i].offset) return -1;

		i=0;
		while ((i<node.size()) && (p_x>=node[i].offset)) i++;

		new_node=i;
		node.resize(node.size()+1);

		if (loop_begin_node>=new_node) loop_begin_node++;
		if (loop_end_node>=new_node) loop_end_node++;
		if (sustain_loop_begin_node>=new_node) sustain_loop_begin_node++;
		if (sustain_loop_end_node>=new_node) sustain_loop_end_node++;

		for (i=node.size()-1;i>new_node;i--) node[i]=node[i-1];

			set_node_offset(new_node,p_x,p_y);

		return new_node;

	}

	void QtEnvelope::set_loop_begin(int pos) {

		if ((pos<0) || (pos>=node.size())) return;

		loop_begin_node=pos;

		if (loop_end_node<loop_begin_node) loop_end_node=loop_begin_node;


	}

	void QtEnvelope::set_loop_end(int pos) {

		if ((pos<0) || (pos>=node.size())) return;

			loop_end_node=pos;
		
		if (loop_end_node<loop_begin_node) loop_begin_node=loop_end_node;


	}


	void QtEnvelope::set_sustain_loop_begin(int pos) {

		if ((pos<0) || (pos>=node.size())) return;

		sustain_loop_begin_node=pos;

		if (sustain_loop_end_node<sustain_loop_begin_node) sustain_loop_end_node=sustain_loop_begin_node;


	}

	void QtEnvelope::set_sustain_loop_end(int pos) {

		if ((pos<0) || (pos>=node.size())) return;

			sustain_loop_end_node=pos;
		
		if (sustain_loop_end_node<sustain_loop_begin_node) sustain_loop_begin_node=sustain_loop_end_node;


	}


	void QtEnvelope::delete_node(int p_node) {

		if ((node.size()<=min_nodes) || (p_node<=0) || (p_node>=node.size())) return;

		int i;

		if (loop_begin_node>=p_node) loop_begin_node--;
		if (loop_end_node>=p_node) loop_end_node--;
		if (sustain_loop_begin_node>=p_node) sustain_loop_begin_node--;
		if (sustain_loop_end_node>=p_node) sustain_loop_end_node--;

		for (i=p_node;i<node.size()-1;i++) node[i]=node[i+1];

		node.resize(node.size()-1);

	}

	int QtEnvelope::get_node_count() {

		return node.size();
	}

	int QtEnvelope::get_node_height(int p_node) {

		if ((p_node<0) || (p_node>=node.size())) return NO_POINT;

		return node[p_node].value;

	}
	int QtEnvelope::get_node_offset(int p_node) {

		if ((p_node<0) || (p_node>=node.size())) return NO_POINT;

		return node[p_node].offset;
		
	}

	int QtEnvelope::get_loop_begin() {

		return loop_begin_node;	
	}
	int QtEnvelope::get_sustain_loop_begin() {

		return sustain_loop_begin_node;

	}

	int QtEnvelope::get_loop_end() {

		return loop_end_node;

	}
	int QtEnvelope::get_sustain_loop_end() {

		return sustain_loop_end_node;

	}

	bool QtEnvelope::is_loop_enabled() {


		return loop_on;
	}
	bool QtEnvelope::is_sustain_loop_enabled() {

		return sustain_loop_on;

	}

	bool QtEnvelope::is_enabled() {

		return on;
	}


	void QtEnvelope::set_enabled (bool p_enabled) {


		on=p_enabled;
	}

	void QtEnvelope::set_loop_enabled (bool p_enabled) {


		loop_on=p_enabled;
	}

	void QtEnvelope::set_sustain_loop_enabled (bool p_enabled) {


		sustain_loop_on=p_enabled;
	}

	void QtEnvelope::set_max_nodes(int p_max) {

		if (p_max<-1)
			return;

			max_nodes=p_max;

	}

	void QtEnvelope::set_min_nodes(int p_min) {

		min_nodes=p_min;

	}

} // namespace CLAM

