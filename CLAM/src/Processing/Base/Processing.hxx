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


#ifndef _Processing_hxx_
#define _Processing_hxx_


#include "DynamicType.hxx"
#include "Component.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include "ErrProcessingObj.hxx"
#include "PublishedInControls.hxx"
#include "PublishedOutControls.hxx"
#include "PublishedInPorts.hxx"
#include "PublishedOutPorts.hxx"
#include "ProcessingConfig.hxx"


#include <vector>
#include <list>
#include <typeinfo>
#include <string>

namespace CLAM {

	class Processing;
	class Port;
	class InPort;
	class OutPort;
	class ProcessingComposite;

	/**
	 * This is the base of all the CLAM processing object classes.
	 * 
	 * It holds common information to all processings: lists of ports,
	 * lists of controls, name, etc.  */
	class Processing: public Component {
	public:
		/* Processing Object possible execution states.
		 */
		typedef enum {
			Unconfigured=0,
			Disabled,
			Ready,
			Running
		} ExecState;

// Attributes:
	private:
		/** Processing object name */
		std::string mName;

		/** Pointer to the parent (composite) processing object, or 0 */
		ProcessingComposite *mpParent;

		/** Processing object execution state */
		ExecState mState;

	protected:

		/** Status description, for debugging */
		std::string mStatus;

		std::vector<InPort*> mPublishedInPorts;
		std::vector<OutPort*> mPublishedOutPorts;
		std::vector<OutControl*> mPublishedOutControls;
		std::vector<InControl*> mPublishedInControls;

	public:
		/**
		 *   Processing Object Input Interator
		 */
		typedef std::vector<InPort*>::iterator InPortIterator;
		/**
		 *   Processing Object Output Interator
		 */
		typedef std::vector<OutPort*>::iterator OutPortIterator;
		/**
		 *   Processing Object Published In-Controls Interator
		 */
		typedef std::vector<InControl*>::iterator InControlIterator;
		/**
		 *   Processing Object Published Out-Controls Interator
		 */
		typedef std::vector<OutControl*>::iterator OutControlIterator;


		/**
		 *   Processing Object Constant Input Iterator
		 */
		typedef std::vector<InPort*>::const_iterator ConstInPortIterator;
		/**
		 *   Processing Object Constant Output Iterator
		 */
		typedef std::vector<OutPort*>::const_iterator ConstOutPortIterator;
		/**
		 *   Processing Object Published In-Controls Interator
		 */
		typedef std::vector<InControl*>::const_iterator ConstInControlIterator;
		/**
		 *   Processing Object Published Out-Controls Interator
		 */
		typedef std::vector<OutControl*>::const_iterator ConstOutControlIterator;


		/**
		 *  Accessor to the first of the Processing Object Inputs
		 */
		InPortIterator FirstInput()
		{
			return mPublishedInPorts.begin();
		}

		/**
		 *  Accessor to the last of the Processing Object Inputs
		 */
		InPortIterator LastInput()
		{
			return mPublishedInPorts.end();
		}

		/**
		 *  Accessor to the first of the Processing Object Ouputs
		 */
		OutPortIterator FirstOutput()
		{
			return mPublishedOutPorts.begin();
		}

		/**
		 *  Accessor to the last of the Processing Object Outputs
		 */
		OutPortIterator LastOutput()
		{
			return mPublishedOutPorts.end();
		}

		/**
		 *  Accessor to the first of the published In-Controls
		 */
		InControlIterator FirstInControl()
		{
			return mPublishedInControls.begin();
		}
		/**
		 *  Accessor to the last of the published In-Controls
		 */
		InControlIterator LastInControl()
		{
			return mPublishedInControls.end();
		}
		/**
		 *  Accessor to the first of the published Out-Controls
		 */
		OutControlIterator FirstOutControl()
		{
			return mPublishedOutControls.begin();
		}
		/**
		 *  Accessor to the last of the published Out-Controls
		 */
		OutControlIterator LastOutControl()
		{
			return mPublishedOutControls.end();
		}



		/**
		 *  Accessor to the first of the Processing Object Inputs (const)
		 */
		ConstInPortIterator FirstInput() const
		{
			return mPublishedInPorts.begin();
		}
		/**
		 *  Accessor to the last of the Processing Object inputs (const )
		 */
		ConstInPortIterator LastInput() const
		{
			return mPublishedInPorts.end();
		}
		/**
		 *  Accessor to the first of the Processing Object Outputs ( const )
		 */
		ConstOutPortIterator FirstOutput() const
		{
			return mPublishedOutPorts.begin();
		}
		/**
		 *  Accessor to the last of the Processing Object Outputs ( const )
		 */
		ConstOutPortIterator LastOutput() const
		{
			return mPublishedOutPorts.end();
		}
		/**
		 *  Accessor to the first of the published In-Controls (const)
		 */
		ConstInControlIterator FirstInControl() const
		{
			return mPublishedInControls.begin();
		}
		/**
		 *  Accessor to the last of the published In-Controls (const)
		 */
		ConstInControlIterator LastInControl() const
		{
			return mPublishedInControls.end();
		}
		/**
		 *  Accessor to the first of the published Out-Controls (const)
		 */
		ConstOutControlIterator FirstOutControl() const
		{
			return mPublishedOutControls.begin();
		}
		/**
		 *  Accessor to the last of the published Out-Controls (const)
		 */
		ConstOutControlIterator LastOutControl() const
		{
			return mPublishedOutControls.end();
		}


	protected:
		/** Configuration method interface.
		 * The Processing base class forces all the concrete
		 * classes derived from it to implement this method, which
		 * must actually perform the specific configuration
		 * tasks.
		 * <p> Note that the user can not call this method
		 * directly. He will use Configure instead. The argument is
		 * expected to be an object of the necesary concrete configuration
		 * class.
		 * @param Reference to the configuration object.
		 * @return <b>false</b> if the object is not yet fully configured.
		 * (For example, if some important configuration attribute
		 * such as number of ports was set to zero in the config
		 * object)
		 * <p> <b> true </b> if the processing object is left in a
		 * consistent state, and can be executed.
		 * @throw This method must throw a bad_cast exception if the
		 * argument is not an object of the expected configuration class.
		 * */
		virtual bool ConcreteConfigure(const ProcessingConfig&) = 0;

		/**
		 * Processing objects have to redefine this method when starting
		 * them implies some internal changes. ie: adquiring resources.
		 * @returns Whether start changes have been successful
		 */
		virtual bool ConcreteStart() {return true;};


		/**
		 * Helper template to convert a reference to a ProcessingConfig to the concrete
		 * ProcessingConfig specified on the first parameter.
		 * @param concrete The copy destination (it forces the runtime type for abstract)
		 * @param abstract A reference to the configuration to be copied
		 * @pre The object runtime type must be exactly the type required by the first parameter
		 */
		template <typename ConcreteConfig>
		void CopyAsConcreteConfig(ConcreteConfig & concrete, const ProcessingConfig & abstract) const {
			CLAM_ASSERT(typeid(ConcreteConfig)==typeid(abstract), 
				"Configuring a Processing with a configuration not being the proper type.");
			concrete = static_cast<const ConcreteConfig &>(abstract);
		}

		/**
		 * Processing objects have to redefine this method when stoping
		 * them implies some internal changes. ie: releasing resources.
		 * @returns Whether stop changes have been successful
		 */
		virtual bool ConcreteStop() {return true;};

		void SetOrphan();

		/**
		 * An special Configure case for TopLevelProcessing.
		 * @todo review its utility and refactor code duplication
		 */
		void ConfigureOrphan(const ProcessingConfig &c);

		bool AbleToExecute(void) const
		{
			/* this looks more complicated than what it is, but that's because
			 * we have to work around a gcc bug where the running operator +
			 * is called for string + string. ideally, we would just use
			 * CLAM_DEBUG_ASSERT
			 */
			CLAM_BEGIN_DEBUG_CHECK
				if (GetExecState() == Unconfigured ||
				    GetExecState() == Ready)
				{
					std::string err(GetClassName());
					err += ": Do(): Not in execution mode - did you call Start on this "
							"object, the composite it is in, or the ToplevelProcessing singleton?";

					CLAM_DEBUG_ASSERT( false, err.c_str() );
				}
			CLAM_END_DEBUG_CHECK

			return GetExecState() != Disabled;
		}


	public:
		Processing();

		virtual ~Processing();

		/** Method to turn the object into running state.
		 * This method must be called before any call to Do() methods.
		 * @asserts that the processing object is ready
		 */
		void Start(void);

		/** Method to put the object out of running state When in
		 * execution mode, this method must be called before any
		 * further call to Configure() methods
		 * @asserts that the processing object is
		 * runnig (or disabled).
		 */
		void Stop(void);
	

	public:
		bool CanDoUsingPorts()
		{	
			return GetInPorts().AreReadyForReading() && GetOutPorts().AreReadyForWriting();
		}

		//XA: test
		virtual bool CanProcessInplace() {return true;}
		

		/**
		 * Supervised mode execution method (using ports)
		 * @return A boolean telling whether it has been output
		 */
		virtual bool Do(void)=0;

		/** Configuration change method.
		 *  This is the method to be used in order to update the
		 *  configuration of a processing object. Note that the object
		 *  must NOT be running when calling this method.
		 *  This method performs some execution state checkings and
		 *  updates, and calls the ConcreteConfigure method of the
		 *  concrete class to perform the actual configuration.
		 *  @param Reference to the configuration object.
		 *  @throw ErrProcessingObj if the processing object is in
		 *  running or disabled state, or if the argument is not
		 *  an object of the configuration class matching the concrete
		 *  processing class of the processing object.
		 */
		bool Configure(const ProcessingConfig&) throw(ErrProcessingObj);

		/** Configuration getter.
		 * Gets the configuration parameters used to create the object.
		 * @return Const reference to the configuration object. The
		 * usual way to perform a configuration change in the
		 * processing object is to take a copy of this reference, to
		 * change it and to send it with the Configure method.
		 */
		virtual const ProcessingConfig &GetConfig() const = 0;

		/* State getter.
		 * Returns the object execution state. This may be used for
		 * debugging purposes, and should also be used inside the
		 * execution (Do) methods to confirm that the object is either
		 * in running or in disabled state.
		 */
		ExecState GetExecState() const {return mState;}

		void PublishOutPort(OutPort* out);
		void PublishInPort(InPort* in);
		void PublishOutControl(OutControl* out);
		void PublishInControl(InControl* in);
		void LinkOutWithInControl(unsigned outId, Processing* inProc, unsigned inId) const;// throw out_of_range;
		int DoControl(unsigned id, TControlData val) const; // throw out_of_range;
		int SendControl(unsigned id, TControlData val) const; // throw out_of_range;
		InControl* GetInControl(unsigned inId) const;
		OutControl* GetOutControl(unsigned inId) const;

		/** Processing object composite iterator */
		typedef std::list<Processing*>::iterator iterator;

		/** Null iterator, returned by default by non-composite POs. */
		static const iterator null_iterator;

		/** Processing object agregate iterator interface.
		 * <p> Default "leaf" implementation.
		 * <p> This method is overriden in the ProcessingComposite
		 * class.
		 */
		virtual iterator composite_begin() {return null_iterator;}

		/** Processing object agregate iterator interface.
		 * <p> Default "leaf" implementation.
		 * <p> This method is overriden in the ProcessingComposite
		 * class.
		 */
		virtual iterator composite_end() {return null_iterator;}

		virtual void FlattenObjects(std::list<Processing*> &l) {l.push_back(this);}

		/** Attribute access method */
		ProcessingComposite *GetParent() const {return mpParent;}
		/** Attribute access method */
		void SetParent(Processing *p);

		/** Accesor */
		const std::string &GetStatus() const {return mStatus;}

		/** Configuration attribute access method */
		const std::string &GetName() const {return mName;}

		/**
		 * Builds the qualified name of the object.
		 * @return Name of the processing object, after appending to it
		 *	the full path of the compound objects containing this one.
		 */
		virtual std::string GetFullName() const;

		// Input Output related methods.
		// They are not mandatory; a default implementation is given.
		// Most of them need only be implemented to allow certain
		// optimisations in the processing object code.

		/** Method used by ports to notify an externally requested
		 * change in their prototype.
		 */
		virtual bool SetPrototypes() {return false;}

		/** Method which tells the object to forget all the input/output
		 * prototypes, and to revert to the default unoptimised state.
		 */
		virtual bool UnsetPrototypes() {return false;}

		/** This method returns true if the processing object may be
		 * disabled for a certain amount of time. This usually happens
		 * if the object execution methods do not change its
		 * internal state.
		 * @todo: Ports will use this method to know if they may
		 * propagate the disable request to other objects connected to
		 * the inputs of this one.
		 */
		virtual bool MayDisableExecution() const {return false;}

		/** Method used by output ports to notify an external
		 *  execution-disable message.
		 */
		bool DisableExecution() {return false;}

		/** Method used by output ports to notify an external
		    execution-enable message. */
		bool EnableExecution() {return false;}

		/** This is just a  DRAFT.
		 * For some reason the last written data was lost and someone
		 * has requested it to be re-sent. The processing object may:
		 *   - Resend the previous data
		 *   - Ask its inputs for their previous outputs
		 *   - Ignore the request.
		 *   - throw an exception
		 */
		virtual bool LostData(int PortID) {return false;}

		/**
		 * Processings must redefine this function in order to save
		 * its running state.
		 * By defining StoreOn and LoadFrom properly in every Processing
		 * of your system you can freeze the whole processing network
		 * execution status onto an Storage in order to later resume its
		 * execution.
		 * You must to store any variable that is kept as status inside
		 * the processing: ie. control values, history buffers...
		 * @see Storage
		 * @see Component::StoreOn
		 */
		virtual void StoreOn(Storage & store)
		{
			CLAM_ASSERT(false, "Processing::StoreOn() not yet implemented");
		}

		/**
		 * Processings must redefine this function in order to restore
		 * its running state.
		 * By defining StoreOn and LoadFrom properly in every Processing
		 * of your system you can freeze the whole processing network
		 * execution status onto an Storage in order to later resume its
		 * execution.
		 * You must to load any variable that is kept as status inside
		 * the processing: ie. control values, history buffers...
		 * @see Storage
		 * @see Component::LoadFrom
		 */
		virtual void LoadFrom(Storage & store)
		{
			CLAM_ASSERT(false, "Processing::LoadFrom() not yet implemented");
		}

		//---------
		// refactoring ports/controls in progress
		// begin
	public:
		
		PublishedInControls& GetInControls()
		{
			return mInControls;
		}
		
		PublishedOutControls& GetOutControls()
		{
			return mOutControls;
		}
	
		PublishedInPorts& GetInPorts()
		{
			return mInPorts;
		}
		PublishedOutPorts& GetOutPorts()
		{
			return mOutPorts;
		}

	private:
		PublishedInControls mInControls;
		PublishedOutControls mOutControls;
		PublishedInPorts mInPorts;
		PublishedOutPorts mOutPorts;

		// end refactoring in progress
		// ---------

	};


};//namespace CLAM

#endif

