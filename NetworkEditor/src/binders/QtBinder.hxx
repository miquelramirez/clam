#ifndef CLAM_QtBinder_hxx
#define CLAM_QtBinder_hxx

#include <QtGui/QWidget>
#include <QtCore/QVariant>

namespace CLAM { class Network; }
namespace CLAM { class QtBinder; }

/**
	A QtBinder relates widgets of a Qt UI to a CLAM network.
	Subclasses of this class binds
	Usually a binder does the following:
	- Evaluates if it can handle a given widget
	- Does the actual binding

	Doing the binding normally requires:
	- Taking extra information for the binding from
	  the object name, its class or its qt properties.
	- Creating the structures to make the binding such as
	  QObjects that bridge signals and slots or 
	  CLAM processings to safely obtain or send processing
	  and control data from the processing thread.
	- Relates them all.

	Instances of QtBinder subclasses register on a list
	that is processed each time a binding is done.
	You can use convenience static methods to apply them all
	to a given ui recursively.

	Notice that the binder may bind many widgets, or reused
	for several networks/ui's so subclasses should not keep 
	the state of a single binding.
	Instead consider transfer ownership to either, the interface
	using the QObject ownership hierarchy or to the network,
	in the case of creating processings.
	If both worlds reference themselves, ensure that when one
	is destroyed the other don't keep dangling references.

	This class also provides helper methods to do common tasks
	in the sublcasses.

	@ingroup UiBinders
*/
class CLAM::QtBinder
{
public:
	typedef std::list<QtBinder*> Binders;
	static Binders & binders();
	QtBinder();
	virtual ~QtBinder();

	/**
		Binds any handled user interface element to the network.
		@see bind, handle
	*/
	void bindRecursively(QObject * userInterface, CLAM::Network & network, QStringList & errors);

	/**
		Applies bindRecursively to all the registered binders.
	*/
	static void bindAllBinders(QObject * userInterface, CLAM::Network & network, QStringList & errors);

	/**
		Returns a binder that handles the concrete uiElement.
	*/
	QtBinder * findHandler(QObject * uiElement);

	/**
		Returns true if the uiElement is handled by the binder.
		For example when it is of a given type or the name or any property match a given criterion.
	*/
	virtual bool handles(QObject * uiElement) = 0;

	/**
		Do the actual binding between the ui element and the network
		Returns false if the bind could not be made.
		If so, any error message is appended to errors.
	*/
	virtual bool bind(QObject * uiElement, Network & network, QStringList & errors) = 0;

// Helpers for subclass implementation of handles

	template <typename Type>
	static bool isSubClassOf(QObject * object)
	{
		return qobject_cast<Type*>(object) != 0;
	}
	static bool hasProperty(QObject * object, const char * propertyName)
	{
		return object->property(propertyName).isValid();
	}
	template <typename ValueType>
	static bool propertyHasValue(QObject * object, const char * propertyName, const ValueType & value)
	{
		const QVariant & propertyValue = object->property(propertyName);
		if (not propertyValue.isValid()) return false;
		if (not propertyValue.canConvert<ValueType>()) return false;
		// TODO documentation says that convert is safer than canConvert
		// but how do i get the QVariant::Type of ValueType
//		if (not propertyValue.convert(QVariant::<ValueType>())) return false;

		return propertyValue.value<ValueType>() == value;
	}
	template <typename ValueType>
	static ValueType propertyDefault(QObject * object, const char * propertyName, const ValueType & defaultValue)
	{
		const QVariant & propertyValue = object->property(propertyName);
		if (not propertyValue.isValid()) return defaultValue;
		if (not propertyValue.canConvert<ValueType>()) return defaultValue;
		// TODO documentation says that convert is safer than canConvert
		// but how do i get the QVariant::Type of ValueType
//		if (not propertyValue.convert(QVariant::<ValueType>())) return defaultValue;

		return propertyValue.value<ValueType>();
	}

	/**
		Helper that turns a mangled widget name into a network name.
		- Removes the prefix
		- Demangles mangled '.' and ' ' that were turned underlines.
		- Converts the QString into std::string to be used with CLAM
	*/
	static std::string widget2NetworkName(const QString & prefix, const QString & widgetName);

// Helpers for subclass implementation of bind
	/**
		Enables the following idiom:
		@code
		if (error_condition)
			return error(errors, "Message");
		@endcode
	*/
	static bool error(QStringList & errors, const QString & message);

	/**
		Adds an error to errors and returns true if 
		the name processing can not be found on the network.
	*/
	static bool missingProcessing(const std::string & processingName, Network & network, QStringList & errors);

	/**
		Adds an error to errors and returns true if 
		the out port or its containing processing can not be found on the network
		The port is refered as a 'processing.outport' like strings.
	*/
	static bool missingOutPort(const std::string & portName, Network & network, QStringList & errors);
	/**
		Adds an error to errors and returns true if 
		the in control or its containing processing can not be found on the network.
		The control is refered as a 'processing.incontrol' like string.
	*/
	static bool missingInControl(const std::string & controlName, Network & network, QStringList & errors);
};

#endif//CLAM_QtBinder_hxx

