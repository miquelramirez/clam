/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
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

#ifndef __VMQT_PLAYABLE_AUDIO_STEREO_PLOT_H__
#define __VMQT_PLAYABLE_AUDIO_STEREO_PLOT_H__

#include "vmAudioStereoPlot.hxx"

class QComboBox;
class QCheckBox;

namespace CLAM
{
	namespace VM
	{
		class WPlayer;
		class AudioPlayer;

		class PlayableAudioStereoPlot : public AudioStereoPlot
		{
			Q_OBJECT
		public:
			PlayableAudioStereoPlot(QWidget* parent=0);
			~PlayableAudioStereoPlot();

			void SetData(const Audio& chn0, const Audio& chn1, bool update=false);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void setColorSchema(int);
			void muteChannel0(int);
			void muteChannel1(int);

		private:
			QComboBox* mComboBox;
			QCheckBox* mMuteChn0;
			QCheckBox* mMuteChn1;

			WPlayer*     mWPlayer;
			AudioPlayer* mAudioPlayer;

			void InitPlayableAudioStereoPlot();
		};
	}
}

#endif

