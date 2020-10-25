#include "stdafx.h"
#include "Image.h"

// Initialises the image to the given dimensions
void Image::init(unsigned width, unsigned height)
{
	m_width = width;
	m_height = height;
	m_pixels.resize(m_width * m_height);
	clear();
}

// Resets the image to a default colour, maintaining its size
void Image::clear()
{
	unsigned size = m_pixels.size();
	m_pixels.clear();
	m_pixels.resize(size, Colour());
}
