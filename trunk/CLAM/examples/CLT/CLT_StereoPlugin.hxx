#ifndef CLT_STEREOPLUGIN_
#define CLT_STEREOPLUGIN_

#include "CLT_Plugin.hxx"

class CLT_StereoPlugin : public CLT_Plugin
{
public:
		CLT_StereoPlugin(){}
		virtual ~CLT_StereoPlugin(){}
		static Audio       sIBufferL;
		static Audio       sIBufferR;
		static Audio       sOBufferL;
		static Audio       sOBufferR;
		virtual bool Do(Audio &inL,Audio &inR, Audio &outL, Audio &outR)=0; //a implementar pels fills

};


#endif
