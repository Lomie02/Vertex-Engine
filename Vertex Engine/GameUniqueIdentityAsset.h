#pragma once
#include <iostream>
/*
	Gameobjects use generated ids for colour picking & rebuilding parent relationships. 
	sub systems do not require ids but are optional.
*/

class GameUniqueIdentityAsset
{
public:
	/// <summary>
	/// Generate a unique gameobject id
	/// </summary>
	/// <returns></returns>
	static uint32_t GenerateUniqueIdenityIndex() {
		static uint32_t generatedIndex = 1;
		generatedIndex++;
		return generatedIndex;
	}

	/// <summary>
	/// Generate a scene id.
	/// </summary>
	/// <returns></returns>
	static uint32_t GenerateUniqueSceneIdentity() {
		static uint32_t sceneid = 1;
		sceneid++;
		return sceneid;
	}

	/// <summary>
	/// Generate a component id.
	/// </summary>
	/// <returns></returns>
	static uint32_t GenerateUniqueComponentIdentity() {
		static uint32_t compId = 1;
		compId++;
		return compId;
	}

	// Generate a sub system id.
	static uint32_t GenerateSubSystemUniqueIdenity() {
		static uint32_t subSys = 1;
		subSys++;
		return subSys;
	}
};

