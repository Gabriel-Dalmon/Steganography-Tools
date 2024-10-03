#pragma once

class GraphicResource {
public:
	virtual void Draw(HDC compatibleDevice, HDC sourceCompatibleDevice) = 0;
	virtual void Release() = 0;
};