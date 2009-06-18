/*
 * Copyright (c) 2009 Fundació Barcelona Media Universitat Pompeu Fabra
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "MultiLibloSource.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "MultiLibloSource",
	"category", "[plugin] Open Sound Control",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::MultiLibloSource> registrator(metadata);

int CLAM::MultiLibloSource::controls_handler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	CLAM::MultiLibloSource & self = *((CLAM::MultiLibloSource*)user_data);
	/* example showing pulling the argument values out of the argv array */
//	printf("%s <- f:",path);
	for (int i=0;i<argc;i++)
	{
		//TODO: replace by ASSERT?
		if (self._outControls.size()>(unsigned)i)
			CLAM::MultiLibloSource::sendControl(self._outControls[i],argv[i]);
	}
//	printf("\n\n");
//	fflush(stdout);
	return 0;
}

