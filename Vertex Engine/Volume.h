#pragma once
/*
 The Volume class is responsible for post processing effects for Tension2D. These volumes can be created & attached to a camera so they can be used in renderering.

*/

struct ChromaticAberation { // Chromatic
public:
	bool ChromaticEnabled = false;
	float ChromaticIntensity = 0.02f;
};

struct Blur { // Blur 
	bool BlurEnabled = false;
	float BlurIntensity = 0.02f;
};

class Volume
{
public:

	Blur Blur;
	ChromaticAberation ChromaticAberation;
};

