#ifndef CLT_MONOPLUGIN_
#define CLT_MONOPLUGIN_

#include "CLT_Plugin.hxx"

class CLT_MonoPlugin : public CLT_Plugin
{
public:
		CLT_MonoPlugin(){}
		virtual ~CLT_MonoPlugin(){}
		static Audio       sIBuffer;
		static Audio       sOBuffer;
		virtual bool Do(Audio &in, Audio &out)=0; //a implementar pels fills

};


#endif
