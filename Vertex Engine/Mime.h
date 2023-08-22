#pragma once
#include <vector>
#include "VertexComponent.h"
#include "Transform.h"
#include <iostream>
#include "GameObject.h"

/*
	The MimeProfile is a test system that allows fake copies of a GameObject to be made & does not need to be registered to the asset manager. 
	Mimes use the same material as its host & only requires a position for now since the Mine will copy every other transform setting. 

	Pros:
	- Do not need to create new gameobject pointers which can be messy in code.
	- Does not need to be registered to Asset Manager
	- Don't need to allocate anything
	- Less Messy 
	- Mimes can copy its hosts Flipbook animations.
	- Mimes do not need to be deleted since they are not pointers.

	Cons:
	- Doesnt have its own Transform properties seperate from its host besides position.
	- Does not have Independent layering (Mimes share the same Layer as their host)
	- Does not go through the Tension transparent layer sorting stage.
	- Mimes are limited per GameObject. This is so the Asset Manager can sort everything quickly & Mimes can cause possible performance drops.
	- Mimes do not have materials attached to them. They use the same Material as their host.
	- Mimes are not a "Real" GameObject. This means Mimes cannot be linked to Vertex Components/ Physics Systems.
	- Mimes only work in the Tension2D renderer. If you try to create a mime in the Vertex2D renderer it will not work.

	Notes:

	Mimes have alot of strengths but of course many weaknesses come with them. Mimes were originally planned to be a Vertex Component 
	but were changed to be merged into the GameObject class. This made Mimes 10x eaiser to create & use unlike its orignal concept as a Component.
	Having the Mime system as a componenet would have been tedious & messy to even use, first the system would need to be created the same way as a ptr
	& registered to the asset manager like any vertex componenet. The host would then need to be assigned to it manually, making code messy if you were to have alot of 
	gameobjects or mime components.

	The Mime concept that was used instead was a better choice as it is way cleaner & easier to use instead. No assigning hosts, no registering to the Asset Manager
	& no need to even create the profile! Mimes can be created though any object via a function gameobject->InstanceMime(params);, the function requires a 
	name for the Mime (So it can be edited or searched for.) as well as a positon. Currently the Mimes only have their positions seperate from their hosts
	& share the other properties in the hosts transform. 

	Future Features for Mimes:
	- Option to set the Mimes transform to copy its host or be seperate.
	- Option to have Mimes layers be seperate or shared.
	- Mimes to go through the Tension transparency Sort

*/
struct MimeProfile 
{
	bool IsActive;
	bool ShareTransform;
	bool ShareLayer;

	std::string Name;
	Transform transform;
};
