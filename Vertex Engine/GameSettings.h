#pragma once
// ============================================== 
//	GENERAL
// ============================================== 
#define PROJECT_NAME "My Game"
#define PROJECT_RESOLUTION 1920, 1080

#define PROJECT_ASPECT_WIDTH 1920
#define PROJECT_ASPECT_HEIGHT 1080

#define PROJECT_DEVELOPER "Your Name"
#define PROJECT_COMPANY "Your Company"

#define ENGINE_VERSION 0.1.0
#define ENGINE_EDITION "Vertex Engine 2D"

#define PROJECT_ICON "PowerIcon.png"

// Change to true if your game is finished & the game will be put into play mode by itself.
#define FINAL_BUILD false

// ============================================== 
//	SCREEN
// ============================================== 
#define FULLSCREEN false //Doesnt affect the Editor!
#define VSYNC true
#define TARGET_FPS 60
#define TARGET_DELTATIME 1
#define BACKGROUND_COLOUR 0.1f, 0.1f, 0.1f, 0.1f

// ============================================== 
// COLOUR PALETTE FOR BACKGROUND
// ==============================================

/*
	Red = 1.0f, 0.0f, 0.0f, 0.0f
	Green = 0.0f, 1.0f, 0.0f, 0.0f
	Blue = 0.0f, 0.0f, 1.0f, 0.0f

	Yellow = 1.0f, 1.0f, 0.0f, 0.0f
	Pink = 1.0f, 0.0f, 1.0f, 0.0f
	Cyan = 0.0f, 1.0f, 1.0f, 0.0f

	Purple = 0.8f, 0.5f, 1.0f, 0.2f
	Dark Green = 0.0f, 0.5f, 0.3f, 0.2f
	Dark Yellow = 0.5f, 0.5f, 0.0f, 0.2f

	Defualt Grey = 0.1f, 0.1f, 0.1f, 1.0f
*/

// ============================================== 
// EDITOR Personalization
// ============================================== 

#define MAX_CAMERAS 80
#define MAX_ASSETS 80
#define MAX_SCENES 80
#define MAX_UI_CANAVS 80

#define EDITOR_BACKGROUND 0.1f, 0.1f, 0.1f, 1.0f
#define EDITOR_MENU 0.2f, 0.2f, 0.2f, 0.1f

#define EDITOR_TABS 0.0f, 0.0f, 0.1f, 0.2f
#define EDITOR_TABS_SELECT 0.1f, 0.1f, 0.3f, 1.0f

#define EDITOR_BUTTONS_SELECT 0.5f, 0.0f, 0.0f, 0.1f
#define PLAY_MODE_COLOUR 0.5f, 0.1f, 0.0f, 1.0f

// =============================================
// Asset Managers Settings
// =============================================

#define CAMERA_DISTANCE_RENDER_LIMIT 10.0f
