#ifndef __DEBUGSNAPSHOTS__
#define __DEBUGSNAPSHOTS__

#include "SnapshotGenerator.hxx"

using CLAM::DataArray;
using CLAMGUI::showSnapshotArray;


// MRJ:
// Nasty kludge for allowing to display Snapshots from 
// the Visual C++ Debugger expression evaluator
// Things to Note:
// Since I don't have a clue about on which stack we are
// executing the snapshots code, nor about which heap memory
// allocations and deallocations are being performed I cannot
// be extremely optimistic about Snapshots stability nor 
// reliability. At least one is able to close the window, but
// whenever someone tries to interact in some way with the
// snapshot everything goes haywire.
// Usage:
// While on Debug Mode, run in Debug Mode ( F5 ). When the
// program stops on a given breakpoint you may access to
// the "Quick Watch" facility throug Menu Debug -> Quick Watch
// ( or just pressing Shift+F9 ). Then enter an statement on
// the following manner:
//     showSnapshotXXXXXX( reference to model object )
// where XXXXXXX is the class name of the model object to
// be viewed e.g. showSnapshotAudio( anAudio )
// Note that the reference may not be constant.


#define PDSNAPSHOT_PROTOTYPE( type) \
extern  void showSnapshot##type( type& obj );\
extern  void showSnapshot##type( type& obj, const char* msg );\

#define PDSNAPSHOT_IMPLEMENTATION( type) \
void showSnapshot##type( type& obj )\
{\
CLAMGUI::showPDSnapshot<##type>( &obj );\
}\
void showSnapshot##type( type& obj, const char* msg )\
{\
CLAMGUI::showPDSnapshot<##type>( &obj, msg );\
}\

#define ARRAYSNAPSHOT_IMPLEMENTATION( tag, type )\
void showSnapshotArray##tag( type obj )\
{\
CLAMGUI::showSnapshotArray( obj );\
}\

// Yet another nasty kludge so we can invoke showSnapshotArray over
// both constant and non-constant references to array....
// Usage:
// + Wait the debugger to arrive to your breakpoint
// + Invoke the Quick Watch window through Shift+F9
// + make the call depending on wether the reference to the model
//   object is const or non const:
//        showSnapshotArrayNonConst( ref ) -> for non constant references
//        showSnapshotArrayConst( ref ) -> for constant references

#define ARRAYSNAPSHOT_PROTOTYPE( tag, type )\
extern  void showSnapshotArray##tag( type obj );\



ARRAYSNAPSHOT_PROTOTYPE( Const, const DataArray& )

ARRAYSNAPSHOT_PROTOTYPE( NonConst, DataArray& )

#endif // DebugSnapshots.hxx
