#pragma once

struct ImageDescriptor {
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	int scaleX = 1;
	int scaleY = 1;
};

class GraphicResource {
public:
	virtual void Draw(HDC compatibleDevice, HDC sourceCompatibleDevice) = 0;
};

class Image : public GraphicResource {
public:
	Image();
	~Image();
	int Init(Bitmap* bitmap, ImageDescriptor* imageDescriptor, HDC compatibleDevice);
	void Draw(HDC compatibleDevice, HDC sourceCompatibleDevice) override;
private:
	HBITMAP m_hGraphicResource;
	bool m_isScaled;
	int m_width;
	int m_height;
	int m_destinationX;
	int m_destinationY;
	int m_destinationWidth;
	int m_destinationHeight;
};