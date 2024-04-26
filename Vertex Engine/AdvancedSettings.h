#pragma once
/*
	ADVANCED SETTINGS

		Only change these settings if you understand what they mean.

		http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/ Keeping Mouse to centre of screen.

		To future me: Stop being lazy & fix your goofy ahh mouse centering 
*/

//===============================================
// TENSION RENDERER
//===============================================
#define RENDER_DEPTH_TEST false		// Tension will render the depth test to the screen
#define TENSION_TRANSPARENT_LAYER_SORTING true // Tensions transparent layer sorting is an experimental feature that may be broken
#define USE_DEPTH_TESTING false

//==============================================
// OPEN EDITOR SETTINGS
//==============================================
#define USE_OPEN_EDITOR false

// Allows you to toggle between using the default scene setup process in application.cpp or the Vertex Workspace method for creating your scenes. 
#define USE_VERTEX_WORK_SPACE true
