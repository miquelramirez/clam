#include "MainWindow.hxx"


MainWindow::~MainWindow()
{
	QSettings settings;
	settings.setValue("RecentFiles",_recentFiles);
	settings.setValue("DockWindowsState", saveState());
}

