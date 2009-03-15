#include "TypedInControl.hxx"
#include "MIDIMessage.hxx"

namespace CLAM {

template<>
const std::string TypedInControl<MIDI::Message>::GetLastValueAsString() // TODO: Use plugins as soon we start to use non streamable types
{
// 	TODO: move MIDI::Message as streameable type?. Warning: TypeInfo<TypedControlData>::StorableAsLeaf affects DynamicTypes Store/Load
	StaticTrue staticTrue;
	return GetLastValueAsString(&staticTrue);
}


}