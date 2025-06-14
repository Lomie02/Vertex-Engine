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

#include "AudioSource.h"
#include "RigidBody.h"

#include "vGameObject.h"
#include "Time.h"
#include "RectTransform.h"
#include "Camera.h"
#include "Transform.h"
#include "SpriteRenderer.h"

enum class EditorMode {
	EDITOR = 0,
	PLAY,
	EDITOR_PLAY,
	EDITOR_PAUSED,
};

struct RunTimeMode {
public:
	EditorMode EditorMode;
};

#define COLOUR_RED 1.0f,0.0f,0.0f