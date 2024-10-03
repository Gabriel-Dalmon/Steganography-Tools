#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

// External libraries
#include <windows.h>
#include <memory>
#include <unordered_set>
#include <stdio.h>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


// TODO: organize pre-compiled headers into sub categories headers (ex: ui.h, file_management.h, steganography.h, etc)

// Forward declarations
template<typename WindowType>
class AbstractWindow;
struct WindowDescriptor;
class Window;
struct TextInputDescriptor;
class TextInput;
struct ButtonDescriptor;
class Button;
class GraphicResource;
struct ImageDescriptor;
class Image;
class Bitmap;
class File;
class App;

#define MAIN_WINDOW_ID 1

#include "Window.h"
#include "Button.h"
#include "TextInput.h"
#include "Image.h"
#include "Bitmap.h"
#include "File.h"
#include "BitUtils.h"
#include "Steganography.h"
#include "App.h"
