#pragma once
#include <AL/al.h>

#define al_check(command) command; assert(alGetError() == AL_NO_ERROR);