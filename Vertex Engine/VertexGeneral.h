#pragma once
#include "PlayerController.h"
#include "Animator.h"
#include "Clip.h"
#include "GameObject.h"

#include "Camera.h"
#include "Graphics.h"
#include "Cursor.h"
#include "Input.h"

#include "Flipbook.h"
#include "AnimationClip.h"
#include "Volume.h"
#include "Random.h"

#include "NavAgent.h"
#include "NavigationGrid.h"
#include "AudioSource.h"

enum EditorMode {
	EDITOR = 0,
	PLAY,
	EDITOR_PLAY,
	EDITOR_PAUSED,
};

#define COLOUR_RED 1.0f,0.0f,0.0f