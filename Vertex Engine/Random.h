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

/// <summary>
/// Vertex's random number generator class. 
/// </summary>
class Random 
{
public:
	/// <summary>
	/// Returns any random number in the Int type.
	/// </summary>
	/// <returns> Random Int</returns>
	static int RandomInt() { return rand(); };
	/// <summary>
	/// Returns a random number within the given min and max numbers.
	/// </summary>
	/// <param name="Min Number"></param>
	/// <param name="Max number"></param>
	/// <returns></returns>
	static int RandomIntRange(int min, int max) { return min + rand() % max; }; // Returns a random int within a range
	/// <summary>
	/// Returns a random float in the given range.
	/// </summary>
	/// <param name="Min number in the range"></param>
	/// <param name="Max number in the range"></param>
	/// <returns></returns>
	static float RandomFloatRange(int min, int max) { return static_cast <float>(RandomIntRange(min, max)) + (rand()) / static_cast <float> (RAND_MAX);}; // Returns a random float within a range
	/// <summary>
	/// Returns a random float.
	/// </summary>
	/// <returns>Result</returns>
	static float RandomFloat() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);}; // Returns a random number (float)
	/// <summary>
	/// Returns a random boolean.
	/// </summary>
	/// <returns>Bool</returns>
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

