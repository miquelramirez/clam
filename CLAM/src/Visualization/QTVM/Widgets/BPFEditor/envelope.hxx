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
                          envelope.h  -  description
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

#ifndef ENVELOPE_H
#define ENVELOPE_H

/**envelope?
  *@author Juan Linietsky
  */

/******************************
 envelope.h
 ----------

Proovides an envelope, and basic functions
for it that can be used for both player
and interface
********************************/

#include <vector>

struct Envelope {

	enum {

		NO_POINT = -32768,
		INFINITE_NODES=-1
	};

private:

	struct Node {

		int offset;
		int value;
	};

	std::vector <Node> node;

	bool on;

	bool loop_on;

	int loop_begin_node;
	int loop_end_node;

	bool sustain_loop_on;
	int sustain_loop_begin_node;
	int sustain_loop_end_node;

	bool envelope_signed;

	int fadeout;

	int max_value;
	int min_value;

	int max_nodes; //-1 for infinite
	int min_nodes;

	///////// METHODS /////////////

public:



	//max and min height for nodes
	void set_max_value(int p_max) { max_value=p_max; }
	void set_min_value(int p_min) { min_value=p_min; }

	int get_max_value() { return max_value; }
	int get_min_value() { return min_value; }
	int get_max_nodes() { return max_nodes; }
	int get_min_nodes() { return min_nodes; }

	//node management
	int get_node_count();
	void set_node_offset(int p_node,int p_x,int p_y);
	int add_node_at_offset(int p_x,int p_y);
	void delete_node(int p_node);
	int get_node_height(int p_node);
	int get_node_offset(int p_node);

	//get envelope val at pos
	float get_height_at_pos(float p_pos); //interpolated height


	//loop point management
	void set_loop_begin(int p_node);
	void set_loop_end(int p_node);

	void set_sustain_loop_begin(int p_node);
	void set_sustain_loop_end(int p_node);

	int get_loop_begin();
	int get_sustain_loop_begin();

	int get_loop_end();
	int get_sustain_loop_end();

	void set_loop_enabled (bool p_enabled);
	bool is_loop_enabled();
	void set_sustain_loop_enabled (bool p_enabled);
	bool is_sustain_loop_enabled();
	void set_enabled (bool p_enabled);
	bool is_enabled();

	void set_max_nodes(int p_max); // -1 infinite nodes
	void set_min_nodes(int p_min);

	void reset();

	Envelope();

};
#endif
