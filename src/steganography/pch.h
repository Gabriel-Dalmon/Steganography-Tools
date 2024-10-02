#pragma once

// External libraries
#include <windows.h>
#include <memory>
#include <unordered_set>
#include <stdio.h>


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

#include "Window.h"
#include "Button.h"
#include "TextInput.h"
#include "Image.h"
#include "Bitmap.h"
#include "File.h"