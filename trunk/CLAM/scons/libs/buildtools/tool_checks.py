tool_checks = dict()

def check_pkg_config(context, *args, **kwords):
	context.Message( 'Checking for pkg-config... ' )
	ret, _  = context.TryAction('pkg-config --help') 
	context.Result( ret )
	return ret

tool_checks['check_pkg_config'] = check_pkg_config

def check_fltk_config( context, *args, **kwargs) :
	context.Message( 'Checking for fltk-config...' )
	res, _ = context.TryAction('fltk-config --api-version')
	context.Result(res)
	return res

tool_checks['check_fltk_config'] = check_fltk_config
