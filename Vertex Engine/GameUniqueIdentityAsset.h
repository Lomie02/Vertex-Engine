#pragma once
#include <iostream>

// Unique Numbers are used by Vertex for many systems. All new gameobjects created must have a Identity.
class GameUniqueIdentityAsset
{
public:
	/// <summary>
	/// Generate a number for Identity
	/// </summary>
	/// <returns></returns>
	static uint32_t GenerateUniqueIdenityIndex() {
		static uint32_t generatedIndex = 1;
		generatedIndex++;
		return generatedIndex;
	}
};

