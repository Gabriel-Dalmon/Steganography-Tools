#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

// External libraries
#include <windows.h>
#include <memory>
#include <unordered_set>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <shobjidl_core.h>
#include <cwchar>

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#endif

// Forward declarations




#define MAIN_WINDOW_ID 106
#define RELEASE(p) {if (p){ p->Release(); p = nullptr;}}


#include "Utils.h"
#include "Graphics.h"
#include "Steganography.h"
#include "Core.h"