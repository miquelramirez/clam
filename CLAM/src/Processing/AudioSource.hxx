#ifndef AudioSource_hxx
#define AudioSource_hxx

#include "Processing.hxx"
#include "AudioOutPort.hxx"

#include <sstream>

namespace CLAM
{

	class AudioSource : public Processing
	{
	public:
		struct Port
		{
			const float* mFloatBuffer;
			const double* mDoubleBuffer;
			unsigned mBufferSize;
			AudioOutPort* mPort;

			Port()
			: mFloatBuffer(0), mDoubleBuffer(0), mBufferSize(0), mPort(0)
			{
			}

			explicit Port(AudioOutPort* p)
			: mFloatBuffer(0), mDoubleBuffer(0), mBufferSize(0), mPort(p)
			{
			}
		};
		typedef std::vector<Port> Ports;

	private:

		class Config : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, int, NSources);
			~Config();
		protected:
			void DefaultInit()
			{
				AddAll();
				UpdateData();
				SetNSources(1);
			};
			void LoadFrom(Storage & storage)
			{
				ProcessingConfig::LoadFrom(storage);
				if (not HasNSources())
				{
					AddNSources();
					UpdateData();
					SetNSources(1);
				}
			}
		};

		private:
			Config _config;
			Ports  _ports;

		public:
			AudioSource(const ProcessingConfig & config=Config())
			{
				//After being dropped it is ready to run as it does not need any configuration at all
				//SetExecState(Ready);
				Configure( config );

				// default constructed with 1 port
				ResizePorts(1);
			}

			~AudioSource()
			{
				for (unsigned port = 0; port < _ports.size(); ++port)
					delete _ports[port].mPort;
			}

			void SetFrameAndHopSize(const int val, unsigned index)
			{
				CLAM_ASSERT(index < _ports.size(), "AudioOutPort index out of range");
				Port& port = _ports[index];
				port.mPort->SetSize(val);
				port.mPort->SetHop(val);
			}

			void SetExternalBuffer(const float* buf, unsigned nframes, unsigned index);
			void SetExternalBuffer(const double* buf, unsigned nframes, unsigned index);

			bool Do();

			const char* GetClassName() const { return "AudioSource";}

			const ProcessingConfig & GetConfig() const
			{
				return _config;
			}

			bool ConcreteConfigure(const ProcessingConfig& config)
			{
				CopyAsConcreteConfig(_config, config);
				unsigned sources = _config.GetNSources();

				ResizePorts(sources);

				return true;
			}

			Ports& GetPorts() { return _ports; }
			const Ports & GetPorts() const { return _ports; }

		private:
			std::string const Portname(unsigned port) const
			{
				std::ostringstream os;
				os << port + 1; //to make ports one based (when viewed in jack)
				return os.str();
			}

			void ResizePorts(unsigned sources)
			{
				if (sources == _ports.size())
					return;

				for (unsigned port = sources; port < _ports.size(); ++port)
					delete _ports[port].mPort;

				unsigned oldSize = _ports.size();
				_ports.resize(sources);

				for (unsigned port = oldSize; port < sources; ++port)
					_ports[port] = Port(new AudioOutPort(Portname(port), this));
			}


	};

} //namespace CLAM

#endif
