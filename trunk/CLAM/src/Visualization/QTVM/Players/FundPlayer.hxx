 #ifndef __FUNDPLAYER__
 #define __FUNDPLAYER__

 #include "Segment.hxx"
 #include "Player.hxx"

 namespace CLAM
 {
	namespace VM
	{
		class FundPlayer : public Player
		{
			public:
				FundPlayer();
				virtual ~FundPlayer();

				void SetData(const Segment& segment);

			protected:
				void thread_code();

			private:
				Segment _segment;
		};
	}
 }

#endif



