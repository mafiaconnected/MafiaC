#pragma once

class CColor
{
public:
	float r, g, b;

	CColor(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	CColor(int hex)
	{
		this->r = (hex >> 16) & 0xFF;
		this->g = (hex >> 8) & 0xFF;
		this->b = hex & 0xFF;
	}

	int ToHEXColor()
	{
		int rgb = r;
		rgb = (rgb << 8) + g;
		rgb = (rgb << 8) + b;

		return rgb;
	}
};

