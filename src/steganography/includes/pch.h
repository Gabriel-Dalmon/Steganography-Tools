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
template<typename WindowType>
class CRTPComponent;
class Component;
struct WindowDescriptor;
struct WindowClassDescriptor;
class Window;
struct ComponentDescriptor;
template<typename ComponentType>
struct GetComponentDescriptorType;
struct TextInputDescriptor;
class TextInput;
struct FileInputDescriptor;
class FileInput;
struct ButtonDescriptor;
class Button;
class GraphicResource;
struct ImageResourceDescriptor;
class ImageResource;
class Bitmap;
class File;
class App;

#define MAIN_WINDOW_ID 106
#define RELEASE(p) {if (p){ p->Release(); p = nullptr;}}


#include "Core.h"
#include "Graphics.h"
#include "Utils.h"
#include "Steganography.h"