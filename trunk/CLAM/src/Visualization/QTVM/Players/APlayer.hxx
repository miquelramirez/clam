 #ifndef __APLAYER__ 
 #define __APLAYER__ 
 
#include <vector>
#include "Audio.hxx"
#include "Player.hxx"
 
 namespace CLAM
 {
	namespace VM
	{
		class APlayer : public Player
		{
			public:
				APlayer();
				virtual ~APlayer();
				
				void SetData(std::vector<Audio> data);

				void SetLeftChannelMuted(bool b);
				void SetRightChannelMuted(bool b);
				
			protected:
				void thread_code();
				
			private:
				Audio _leftChannel;
				Audio _rightChannel;

				bool _muteLeft,_muteRight;

				bool isMutedLChannel();
				bool isMutedRChannel();
		};
	}
 }
 
 #endif
 
