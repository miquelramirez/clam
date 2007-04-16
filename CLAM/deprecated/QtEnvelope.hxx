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

#ifndef QTENVELOPE_H
#define QTENVELOPE_H

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
#include "DataTypes.hxx"

namespace CLAM
{

struct QtEnvelope {

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

	int maxY_value;
	int minY_value;
	int maxX_value;
	int minX_value;

	int max_nodes; //-1 for infinite
	int min_nodes;

	///////// METHODS /////////////

public:



	//max and min height for nodes
	void set_maxY_value(int py_max) { maxY_value=py_max; }
	void set_minY_value(int py_min) { minY_value=py_min; }
	void set_maxX_value(int px_max) { maxX_value=px_max; }
	void set_minX_value(int px_min) { minX_value=px_min; }

	int get_maxY_value() { return maxY_value; }
	int get_minY_value() { return minY_value; }
	int get_maxX_value() { return maxX_value; }
	int get_minX_value() { return minX_value; }
	int get_max_nodes() { return max_nodes; }
	int get_min_nodes() { return min_nodes; }

	//node management
	int get_node_count();

	/* TODO:
	 * p_x and p_y were originally ints
	 * - changed to TData to match with CLAM
	 * comparisons for equality are now dangerous
	 */ 
	void set_node_offset(int p_node,int p_x, int p_y);
	int add_node_at_offset(int p_x, int p_y);
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

	QtEnvelope();

};

} // namespace CLAM
#endif

