tool_checks = dict()

def check_pkg_config(context, *args, **kwords):
	context.Message( 'Checking for pkg-config... ' )
	ret, _  = context.TryAction('pkg-config --help') 
	context.Result( ret )
	return ret

tool_checks['check_pkg_config'] = check_pkg_config

