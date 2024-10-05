#pragma once

// External includes
#include <windows.h>
#include <cwchar>
#include <filesystem>
#include <unordered_set>

// Macros
#define MAIN_WINDOW_ID 106

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

// Includes
#include "Graphics/Component.h"
#include "Graphics/ComponentDescriptorMap.h"

#include "Graphics/Window.h"
#include "Graphics/Button.h"
#include "Graphics/TextInput.h"
#include "Graphics/FileInput.h"

#include "Graphics/GraphicResource.h"
#include "Graphics/ImageResource.h"
