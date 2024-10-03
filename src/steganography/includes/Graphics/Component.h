#pragma once

class Component {
public:

	virtual void Release() = 0;
	void SetInstance() {
		SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	};
protected:
	HWND m_hWnd = nullptr;
};


template<typename DerivedWindowType>
class CRTPComponent : public Component {
public:
	// Doesn't match the current Window Class design :
	//virtual void Init(Window* parentWindow, GetComponentDescriptorType<DerivedWindowType>::type* pComponentDescriptor) = 0; 
	static DerivedWindowType* GetInstance(HWND hWnd) {
		return reinterpret_cast<DerivedWindowType*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		//return (instance->type == typeid(DerivedWindowType)) ? instance : nullptr;
	};

protected:
	//type_info type = typeid(DerivedWindowType);
private:
	//CRTPComponent();
	friend DerivedWindowType;
};
