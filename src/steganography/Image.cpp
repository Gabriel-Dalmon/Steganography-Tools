#include "pch.h"

Image::Image()
{
	m_hGraphicResource = nullptr;
	m_width = 0;
	m_height = 0;
	m_destinationX = 0;
	m_destinationY = 0;
	m_destinationWidth = 0;
	m_destinationHeight = 0;
	m_isScaled = false;
}

Image::~Image()
{
}

int Image::Init(Bitmap* bitmap, ImageDescriptor* imageDescriptor, HDC compatibleDevice)
{
	const auto& bitmapInfoHeader = bitmap->GetMapInfo();
	m_width = bitmapInfoHeader.biWidth;
	m_height = bitmapInfoHeader.biHeight;
	m_destinationX = imageDescriptor->x;
	m_destinationY = imageDescriptor->y;
	if (imageDescriptor->width == 0 && imageDescriptor->height == 0) {
		m_destinationWidth = m_width * imageDescriptor->scaleX;
		m_destinationHeight = m_height * imageDescriptor->scaleY;
	}
	else {
		m_destinationWidth = imageDescriptor->width;
		m_destinationHeight = imageDescriptor->height;
		
	}
	if(m_destinationWidth != m_width || m_destinationHeight != m_height) m_isScaled = true;

	byte* pixelBuffer = bitmap->GetPixelsBuffer();
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader = bitmapInfoHeader;
	m_hGraphicResource = CreateDIBitmap(
		compatibleDevice,
		&bitmapInfoHeader,
		CBM_INIT,
		pixelBuffer,
		&bitmapInfo,
		DIB_RGB_COLORS
	);
	return 0;
}

void Image::Draw(HDC compatibleDevice, HDC sourceCompatibleDevice)
{
	SelectObject(sourceCompatibleDevice, m_hGraphicResource);
	if (m_isScaled) {
		StretchBlt(
			compatibleDevice,
			m_destinationX,
			m_destinationY,
			m_destinationWidth,
			m_destinationHeight,
			sourceCompatibleDevice,
			0,
			0,
			m_width,
			m_height,
			SRCCOPY
		);
	}
	else {
		BitBlt(
			compatibleDevice,
			m_destinationX,
			m_destinationY,
			m_width,
			m_height,
			sourceCompatibleDevice,
			0,
			0,
			SRCCOPY
		);
	}

}
