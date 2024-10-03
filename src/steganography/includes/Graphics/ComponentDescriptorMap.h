#pragma once

struct ComponentDescriptor {
};

template <typename ComponentType>
struct GetComponentDescriptorType {
	using type = ComponentDescriptor;
};

template <>
struct GetComponentDescriptorType<Button> {
	using type = ButtonDescriptor;
};

template <>
struct GetComponentDescriptorType<TextInput> {
	using type = TextInputDescriptor;
};

template <>
struct GetComponentDescriptorType<Window> {
	using type = WindowClassDescriptor;
};
