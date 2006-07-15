#include "NetworkCanvas.hxx"

NetworkCanvas::~NetworkCanvas()
{
	for (unsigned i = 0; i<_portWires.size(); i++)
		delete _portWires[i];
	for (unsigned i = 0; i<_controlWires.size(); i++)
		delete _controlWires[i];
}



