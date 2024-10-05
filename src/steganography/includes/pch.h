#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#endif


#include "Utils.h"
#include "Graphics.h"
#include "Steganography.h"
#include "Core.h"