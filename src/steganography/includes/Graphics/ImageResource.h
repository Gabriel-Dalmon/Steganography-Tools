#pragma once

struct ImageResourceDescriptor {
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	int scaleX = 1;
	int scaleY = 1;
};

class ImageResource : public GraphicResource {
public:
	ImageResource();
	~ImageResource();
	int Init(Bitmap* bitmap, ImageResourceDescriptor* imageDescriptor, HDC compatibleDevice);
	void Release();
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