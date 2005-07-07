generic_checks = dict()

def pkg_config_check_existence(context, *args, **kwargs):
	name = kwargs['name']
	context.Message( 'Checking for %s... ' % name )
	ret = context.TryAction('pkg-config --exists \'%s\'' % name)[0]
	context.Result( ret )
	return ret

generic_checks['pkg_config_check_existence'] = pkg_config_check_existence
