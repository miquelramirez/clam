/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "RegisterConfiguratorLaunchers.hxx"

// audio
#include <CLAM/MonoAudioFileReaderConfig.hxx>
#include <CLAM/MonoAudioFileWriterConfig.hxx>
#include <CLAM/MultiChannelAudioFileReaderConfig.hxx>
#include <CLAM/MultiChannelAudioFileWriterConfig.hxx>
STANDARD_PROCESSING_CONFIG_REGISTER(MonoAudioFileReaderConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(MonoAudioFileWriterConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(MultiChannelAudioFileReaderConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(MultiChannelAudioFileWriterConfig);
