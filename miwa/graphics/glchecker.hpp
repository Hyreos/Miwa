#pragma once

#include "glloader.hpp"
#include <functional>
#include <cassert>

#define gl_check(command) command; assert(glGetError() == GL_NO_ERROR);