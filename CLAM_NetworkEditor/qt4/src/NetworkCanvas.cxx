#include "NetworkCanvas.hxx"

NetworkCanvas::~NetworkCanvas()
{
	clear();
}

void NetworkCanvas::clear()
{
	clearWires();
	for (unsigned i = 0; i<_processings.size(); i++)
		delete _processings[i];
	_processings.clear();
	update();
}

void NetworkCanvas::clearWires()
{
	for (unsigned i = 0; i<_portWires.size(); i++)
		delete _portWires[i];
	_portWires.clear();
	for (unsigned i = 0; i<_controlWires.size(); i++)
		delete _controlWires[i];
	_controlWires.clear();
}




