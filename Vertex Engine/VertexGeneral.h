#pragma once
#include "PlayerController.h"
#include "Animator.h"
#include "Clip.h"
#include "GameObject.h"
#include "Camera.h"
#include "Graphics.h"

enum EditorMode {
	EDITOR = 0,
	PLAY,
	EDITOR_PLAY,
	EDITOR_PAUSED,
};