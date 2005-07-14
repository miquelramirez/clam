def setup_core_environment( core_env, conf ) :
	if core_env['xmlbackend'] == 'xercesc' :
		failed = False	
		if not failed and not conf.CheckCXXHeader('xercesc/util/PlatformUtils.hpp') :
			print "Could not find xerces c headers! Defaulting to the null xml backend"
			failed = True
		if not failed and not conf.check_xerces_c( conf ) :
			print "xerces c code compile/link/run test failed! Check config.log for details..."
			failed = True
		
		if not failed :
			core_env.Append( CPPFLAGS=['-DUSE_XERCES_TRUE', '-DCLAM_USE_XML'] )
		else :
			core_env['xmlbackend'] = 'none'
			
	elif core_env['xmlbackend'] == 'xmlpp' :
		failed = False
		if core_env['pkg_config_available'] :
			res = conf.pkg_config_check_existence( conf, name='libxml++-2.6' )
			if res :
				core_env.ParseConfig( 'pkg-config --cflags --libs libxml++-2.6' )
			else :
				failed = True
		else :
			failed = True
			
		if not failed and not conf.check_xmlpp( conf ) : 
			print "\tERROR: libxml++ code compile/link/run test failed! Check config.log for details"
			failed = True
		
		if not failed:
			core_env.Append( CPPFLAGS=['-DUSE_XMLPP_TRUE','-DCLAM_USE_XML'] )
		else :
			core_env['xmlbackend'] = 'none'	

	if core_env['with_ladspa_support'] :
		result = conf.CheckCHeader( 'ladspa.h' )
		if not result :
			print "ladspa SDK header was not found"
			print "Either install ladspa SDK or disable the option by issuing the option"
			print "$ scons with_ladspa_support=no"
			return False
			#core_env['with_ladspa_support'] = False
		result = conf.check_ladspa()
		if not result :
			print "ladspa SDK compile test failed! Check config.log for details"
			print "Check that your installed ladspa headers match the version required by CLAM"
			return False			

		core_env.Append( CPPFLAGS=['-DUSE_LADSPA=1'])
	
	if core_env['with_osc_support'] :
		result = conf.CheckCXXHeader( 'oscpack/ip/NetworkingUtils.h' )
		if not result :	
			print "liboscpack headers not found!"
			print "Either install liboscpack or disable OSC support by issuing"
			print "$scons with_osc_support=no"
			return False
		result = conf.CheckLibWithHeader( 'oscpack', 'oscpack/ip/NetworkingUtils.h', 'cxx', call='InitializeNetworking();')
		if not result :
			print "liboscpack binaries not found!"
			print "Either install liboscpack or disable OSC support by issuing"
			print "$scons with_osc_support=no"
			return False
		result = conf.check_oscpack()
		if not result :
			print "liboscpack compile/link/run test failed! check config.log for details..."
			print "Either install liboscpack or disable OSC support by issuing"
			print "$scons with_osc_support=no"
			return False
		core_env.Append(CPPFLAGS='-DUSE_OSCPACK=1')	

	return True

