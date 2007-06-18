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


// Controls
#include <CLAM/AutoPanner.hxx>
#include <CLAM/FlagControl.hxx>
#include "Random.hxx"
#include "OneOverF.hxx"
#include <CLAM/Fundamental2Control.hxx>
#include <CLAM/ControlMapper.hxx>
#include <CLAM/ControlPrinter.hxx>
#include <CLAM/ControlScaler.hxx>
#include "ControlSurface.hxx"
#include <CLAM/ControlSource.hxx>
#include <CLAM/ControlTrace.hxx>
#include <CLAM/OutControlSender.hxx>
STANDARD_PROCESSING_CONFIG_REGISTER(AutoPannerConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlPrinterConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlScalerConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlSourceConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlSurfaceConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlTraceReaderConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(ControlTraceWriterConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(FlagControlConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(Fundamental2ControlConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(OneOverFConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(OutControlSenderConfig);
STANDARD_PROCESSING_CONFIG_REGISTER(RandomConfig);
