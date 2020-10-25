#pragma once

// Structure holding RGBA colour components
struct Colour
{
	unsigned char r, g, b, a;
	Colour(unsigned char r_ = 0, unsigned char g_ = 0, unsigned char b_ = 0) : r(r_), g(g_), b(b_), a(255) {}
	void set(const Colour& col) { r = col.r; g = col.g; b = col.b; a = col.a; }
};

// Wrapper for a 2D array of Colour values to represent an image
class Image
{
public:
	void init(unsigned width, unsigned height);
	bool isInitialised() const { return !m_pixels.empty();  }

	unsigned width() const { return m_width; }
	unsigned height() const { return m_height; }

	// Get/set the Colour value of the pixel with the given indices
	const Colour&	getPixel(unsigned i, unsigned j) const { return m_pixels[i + m_height * j]; }
	void			setPixel(unsigned i, unsigned j, const Colour& col) { m_pixels[i + m_height * j].set(col); }

	void	clear();

private:
	unsigned m_width = 0;
	unsigned m_height = 0;
	
	std::vector<Colour> m_pixels;
};