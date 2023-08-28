#pragma once
#include <stdlib.h>
#include <iostream>

/*
	A basic RNG singlton class for generating random ints & floats using the C++ srand() structure.
	Supported RNG types:
		- Random Int
		- Random Float
		- Random Bool
	
*/
class Random 
{
public:
	static int RandomInt() { return rand(); }; // Returns a random number (int)
	static int RandomIntRange(int min, int max) { return min + rand() % max; }; // Returns a random int within a range

	static float RandomFloatRange(int min, int max) { return static_cast <float>(RandomIntRange(min, max)) + (rand()) / static_cast <float> (RAND_MAX);}; // Returns a random float within a range
	static float RandomFloat() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);}; // Returns a random number (float)
	static bool RandomBool() // returns a random boolean
	{
		int conversion = RandomIntRange(0, 2);

		if (conversion == 0)
			return false;
		else
			return true;
	};

private:
	Random();
};

