#ifndef _ProcessingChain_
#define _ProcessingChain_

#include "ProcessingComposite.hxx"
#include "ProcessingData.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"
#include "InControlTmplArray.hxx"
#include "Factory.hxx"


namespace CLAM{

	/**	Special Processing config that can be used inside a ProcessingChainConfig. It is a
	 *	polymorphic configuration that delegates this polymorphism to one of its members 
	 *	(@see mpConcreteConfig) and has access to the concrete class through its name stored
	 *	in a dynamic attribute (ConcreteClassName)
	 */
	class ProcessingChaineeConfig:public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (ProcessingChaineeConfig, 1,ProcessingConfig);
		/** Name of concrete Config class */
		DYN_ATTRIBUTE (0, public, std::string, ConcreteClassName);
	public:
		/** Initialization for default constructor. All attributes are added, ConcreteClassName is
		 *	set to "Unknown" and pointer to concrete configuration is set to null. 
		 */
		void DefaultInit();
		/** Initialization for copy constructor. All dynamic attributes are added and copied from
		 *	original configuration. Concrete Configuration is set 'by hand'.
		 */
		void CopyInit(const ProcessingChaineeConfig& originalConfig);
		/** Overriding virtual method in base class to store concrete configuration by hand as it
		 *	is not a dynamic attribute.
		 */
		void StoreOn(Storage & s) const; 
		/** Overriding virtual method in base class to load concrete configuration by hand as it
		 *	is not a dynamic attribute.
		 */
		void LoadFrom(Storage& s);
		/** Returns the concrete configuration as a reference to the base class.*/
		ProcessingConfig& GetConcreteConfig() const {return *mpConcreteConfig;}
		/** Sets the concrete configuration, passing a reference to the base class. Note
		 *	though, that the actual object referenced must be a concrete configuration of
		 *	the same class indicated by the ConcreteClassName attribute.
		 */
		void SetConcreteConfig(const ProcessingConfig& cfg)
		{
			AddConcreteConfig();
			*mpConcreteConfig=cfg;
		}

		/** Virtual destructor. Deletes pointer to concrete configuration */
		virtual ~ProcessingChaineeConfig();

		/** Adds a new instantiated concrete configuration using the Concrete Class name as a
		 *	type selector (ConcreteClassName must be set in advanced) deleting any previously
		 *	existing concrete configuration.
		 */
		void AddConcreteConfig()
		{
			if(mpConcreteConfig) 
				delete mpConcreteConfig;
			mpConcreteConfig=InstantiateConcreteConfig();
		}

		
	protected:
		/** Instantiates a concrete configuration using input string as a type selector. */
		ProcessingConfig* InstantiateConcreteConfig(const std::string& type);

		/** Instantiates a concrete configuration using the ConcreteClassName attribute as a
		 *	type selector (ConcreteClassName must be set in advanced)
		 */
		ProcessingConfig* InstantiateConcreteConfig()
		{
			return InstantiateConcreteConfig(GetConcreteClassName());
		}
		
		/** Actual pointer to the concrete configuration. It is a pointer to base class but the
		 *	object is always an instance of a concrete one.
		 */
		ProcessingConfig* mpConcreteConfig;
		
	};


	/** Configuration for a ProcessingChain. It is basically made of a list of
	 *	children ProcessingConfigs. Pointers are used to be able to handle polymorphism
	 *	on these children. These pointers to a ProcessingConfig are indeed pointers to
	 *	instances of a derived class.
	 */
	class ProcessingChainConfig:public ProcessingConfig
	{
	public:
		typedef std::list<ProcessingChaineeConfig>::iterator iterator;
		typedef std::list<ProcessingChaineeConfig>::const_iterator const_iterator;
		
		DYNAMIC_TYPE_USING_INTERFACE (ProcessingChainConfig, 2,ProcessingConfig);
		/** List of children configurations, a list of pointers to base class is kept */
		DYN_CONTAINER_ATTRIBUTE (0, public, std::list<ProcessingChaineeConfig>, Configurations,Config);
		/** Array of On/off initial values for control*/
		DYN_ATTRIBUTE (1, public, Array<bool>,OnArray);

		
		/** By default all attributes are added. */
		void DefaultInit();
		/** Returns a configuration iterator at the beginning of the list*/
		iterator ConfigList_begin() {return GetConfigurations().begin();}
		/** Returns a configuration iterator at the end of the list*/
		iterator ConfigList_end() {return GetConfigurations().end();}

		/** Returns a configuration iterator at the beginning of the list*/
		const_iterator ConfigList_begin_const() const {return GetConfigurations().begin();}
		/** Returns a configuration iterator at the end of the list*/
		const_iterator ConfigList_end_const() const {return GetConfigurations().end();}


		/** Returns the size of the configuration list*/
		std::size_t ConfigList_size() const {return GetConfigurations().size();}

		


		//TODO: maybe a method for deleting a configuration is also necessary
		
		/** Returns true if there are no configurations */
		bool IsEmpty(){return GetnConfigurations()<=0;}
		/** Accessor to the number of configurations in the list*/
		TSize GetnConfigurations(){return ConfigList_size();}

		/** Virtual destructor. */
		virtual ~ProcessingChainConfig();
	protected:
		/** Adds a configuration at the end of the list. */
		virtual void AddConfiguration(const ProcessingConfig& newConcreteConfig,const std::string& className);

	};
	

	/** ProcessingChain template class. Template parameter U is the type of the input and output
	 *	port (which must be the same and a subclass of ProcessingData). ProcessingChain can be seen
	 *	as a particular case of ProcessingComposite but has Do() and ConcreteConfigure implemented.
	 */
	template <typename U>
	class ProcessingChain:public ProcessingComposite
	{
		typedef ProcessingChain ThisProc;
	public:
		/** Default Constructor */
		ProcessingChain():mChainInput("Input",this,1),mChainOutput("Output",this,1)
		{
			mpConfig=NULL;
			mpOnCtrlArray=NULL;
		}

		/** Destructor. Deletes temporal ProcessingData and Configuration and any previously
		 *	allocated Processing chainee. 
		 */
		virtual ~ProcessingChain()
		{
			int i;
			for(i=0;i<mpTmpDataArray.Size();i++)
				if(mpTmpDataArray[i]) delete mpTmpDataArray[i];
			if (mpConfig) delete mpConfig;
			iterator obj;
			for(obj=composite_begin();obj!=composite_end();obj++)
			{
				delete (*obj);
			}
		}
		
		/** Accessor to On/off control. Turns on the specified control in the array.*/
		void TurnOn(TIndex index)
		{
			(*mpOnCtrlArray)[index].DoControl(1);
		}

		/** Accessor to On/off control. Turns on the specified control in the array.*/
		void TurnOff(TIndex index)
		{
			(*mpOnCtrlArray)[index].DoControl(0);
		}


		/** Concrete start attaches input and output ports in the chain to first and last Processing
		 *	in the chain. Until this moment we cannot be sure that the composite has valid data in
		 *	input and output ports and we don't know which is the last Processing
		 */
		bool ConcreteStart()
		{
			iterator obj;
			
			int i;
			for(i=0;i<mpTmpDataArray.Size();i++)
				if(mpTmpDataArray[i]){
					delete mpTmpDataArray[i];
					mpTmpDataArray[i]=NULL;}
			mpTmpDataArray.SetSize(0);
			U* pCurrentData;
			pCurrentData=new U(mChainInput.GetData());
			mpTmpDataArray.AddElem(pCurrentData);
			for(obj=composite_begin();obj!=composite_end();obj++)
			{
				//connecting ports for non-supervised mode
				(*obj)->GetInPorts().GetByNumber(0).Attach(*pCurrentData);
				if(!(*obj)->CanProcessInplace())
				{
					pCurrentData=new U(mChainInput.GetData());
					mpTmpDataArray.AddElem(pCurrentData);
				}
				(*obj)->GetOutPorts().GetByNumber(0).Attach(*pCurrentData);
			}
			obj=composite_begin();
			(*obj)->GetInPorts().GetByNumber(0).Attach(mChainInput.GetData());
			obj=composite_end();
			obj--;
			(*obj)->GetOutPorts().GetByNumber(0).Attach(mChainOutput.GetData());

			return ProcessingComposite::ConcreteStart();
		}
		
		/** Supervised Do method. Iterates through internal Processing and calls each one's
		 *	Do() method
		 */
		virtual bool Do()
		{
			CLAM_DEBUG_ASSERT(GetExecState() != Unconfigured &&
		                  GetExecState() != Ready,
		                  "ProcessingChain: Do(): Not in execution mode");

			if (GetExecState() == Disabled)
				return true;
			
			bool result=true;
			iterator obj;
			int i=0;
			//We iterate through all chainees and call their Do()
			for (obj=composite_begin(); obj!=composite_end(); obj++,i++)
			{
				if((*mpOnCtrlArray)[i].GetLastValue()||i==0||i==int(composite_size())-1)
				//Note: First and last chainee's will always be active regartheless the value
				//of their On control.
				{
					try {
						result&=(*obj)->Do();
					}
					catch (Err &e)
					{
						if ((*obj)->GetExecState() != Running)
							throw(ErrProcessingObj("Do(): Child is not running",this));
						else
							throw e;
					}
					
				}
					
			}
			return result;
		}

/*		TODO: Is a non-supervised version of Do necessary? bool Do(const U& in, U& out)
*/
		/** Concrete Configuration. Different Processing in the chain are configured from list
		 *	of basic configurations in ProcessingChainConfiguration passed.
		 */
		bool ConcreteConfigure(const ProcessingConfig& c)
		{
			mpConfig=new ProcessingChainConfig(dynamic_cast<const ProcessingChainConfig&>(c));
			bool result=true;
			iterator obj;
			
			/** First we remove existing chainee if any*/
			obj=composite_begin();
			while(obj!=composite_end())
			{
				Remove(*(*obj));
				obj=composite_begin();
			}
			
			ProcessingChainConfig::iterator cfg;
			for(cfg=mpConfig->ConfigList_begin();cfg!=mpConfig->ConfigList_end();cfg++)
			{
				AddChainee((*cfg).GetConcreteClassName());
				obj=composite_end();
				obj--;
				result&=(*obj)->Configure((*cfg).GetConcreteConfig());
			}
			CLAM_ASSERT(mpConfig->GetConfigurations().size()==composite_size(),"Number of configurations should be the same as number of children");
		
			//TODO: right now there is no way to add or remove controls than to instantiate control array again
			CLAM_ASSERT(mpConfig->GetOnArray().Size()==(int)composite_size(),"ProcessingChain::ConcreteConfigure: On array does not have same size as number of configurations");
			TSize nControls=composite_size();
			if(mpOnCtrlArray) delete mpOnCtrlArray;
			mpOnCtrlArray= new InControlTmplArray<ProcessingChain>(nControls,"OnControlArray",this,NULL);
			int i;
			for(i=0;i<nControls;i++)
			{
				(*mpOnCtrlArray)[i].DoControl(mpConfig->GetOnArray()[i]);
			}
			return result;
		}

		const ProcessingConfig& GetConfig() const
		{
			return *mpConfig;
		}

		/** Input port*/
		InPortTmpl<U>     mChainInput;
		/** Output port*/
		OutPortTmpl<U> mChainOutput;

protected:
		/** Adds a Processing chainee at the end of the chain. It instantiates a concrete Processing
		 *	class using input string as a switch.
		 */
		bool AddChainee(const std::string& type)
		{
			typedef CLAM::Factory<Processing> ProcessingFactory;

			// Factory::CreateSafe throws an ErrFactory exception if the key is not
			// valid
			//InsertAndGiveName( *( ProcessingFactory::GetInstance().CreateSafe( type ) ) );
			Insert( *( ProcessingFactory::GetInstance().CreateSafe( type ) ) );
						
			return true;
			
		}
		/** Temporal ProcessingData used as an internal node for intermediate Processing */
		Array<U*> mpTmpDataArray;
		/** Internal configuration. A pointer is used because polymorphism may be used on it */
		ProcessingChainConfig* mpConfig;

		InControlTmplArray<ThisProc> *mpOnCtrlArray;
		
};



}//namespace

#endif
