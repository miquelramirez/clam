#include <QtGui/QMessageBox>
#include <CLAM/Assert.hxx>
#include <sstream>
#if defined(__linux__)
#include <execinfo.h>
#endif

void DumpBacktrace(std::ostream & os)
{
#if defined(__linux__)

	void *bt_array[100];    // 100 should be enough ?!?
	char **bt_strings;
	int num_entries;

	if ((num_entries = backtrace(bt_array, 100)) < 0) {
		os << " Unable to generate a backtrace" << std::endl;
		return;
	}

	if ((bt_strings = backtrace_symbols(bt_array, num_entries)) == NULL) {
		os << " Unable to adquire symbols names for the backtrace" << std::endl;
		return;
	}

	os << "\n Backtrace:\n" << std::endl;
	for (int i = 0; i < num_entries; i++) {
		os << "[" << i << "] " <<  bt_strings[i] << std::endl;
	}
	free(bt_strings);
#else
	os << " Unable to adquire symbols names for the backtrace" << std::endl;
#endif
}



void QClamAssertHandler(const char *message, const char *filename, int lineNumber)
{
	std::ostringstream backtrace;
	DumpBacktrace(backtrace);
	QMessageBox::critical(0, QObject::tr("Program internal error"), QObject::tr(
		"<p>The program arrived to an unexpected state.</p>\n"
		"<p>Congratulations, you located a bug!\n"
		" Please, contribute to improve the program by filling a ticket on CLAM\n"
		" <a href='%1'>bug tracking system</a>.\n"
		" Besides the context of the error, consider to paste the following information into the bug report.\n"
		" Thanks, and sorry for the inconvenience.</p>\n"
		"<pre style='color:red; background-color: white;'>\n"
		"At source file: %2, line %3:\n"
		"%4%5</pre>\n"
	)
		.arg("https://projectes.lafarga.cat/tracker/?atid=171&group_id=24&func=browse")
		.arg(filename)
		.arg(lineNumber)
		.arg(message)
		.arg(backtrace.str().c_str())
	);
	std::exit(-1);
}

class __q_clam_assert_handler_registrator__ {
public:
	__q_clam_assert_handler_registrator__()
	{
		CLAM::SetAssertFailedHandler(QClamAssertHandler);
	}
};

//TODO make it work in NetworkEditor: save the BT in the Network and notify Qt thread
//static __q_clam_assert_handler_registrator__ _theAssertHandlerRegistrator;

