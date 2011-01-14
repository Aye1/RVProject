/*
 * OpenAL cross platform audio library
 *
 * Copyright (C) 1999-2000 by Authors.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307, USA.
 */

#ifndef __AL_ENVIRONMENT_H
#define __AL_ENVIRONMENT_H

#include "Types.h"

ALenvironment *alimCreateEnvironmentIASIG(ALvoid);

ALvoid alimDeleteEnvironmentIASIG(ALenvironment *environment);

ALenvironment *alimAcquireEnvironmentIASIG(ALenvironment *environment);

ALvoid alimReleaseEnvironmentIASIG(ALenvironment *environment);

ALvoid alimEnvironmentfIASIG(ALcontext *context, ALuint environment, ALenum pname, ALfloat value);

ALvoid alimEnvironmentiIASIG(ALcontext *context, ALuint environment, ALenum pname, ALint value);

ALvoid alimGetEnvironmentfIASIG(ALcontext *context, ALuint environment, ALenum pname, ALfloat *value);

ALvoid alimGetEnvironmentiIASIG(ALcontext *context, ALuint environment, ALenum pname, ALint *value);

#endif
