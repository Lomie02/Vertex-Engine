#include "VertexScene.h"
#include <string.h>
#include <iostream>

VertexScene::VertexScene()
{
}

VertexScene::VertexScene(const char _name[])
{
	strcpy_s(m_SceneName,_name);
}

