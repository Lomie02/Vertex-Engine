# Vertex-Engine
Vertex Engine is a custom C++ Game Engine developed using OpenGL

 Features Coming Soon:
 
 VERTEX ENGINE 2D:

next to come:
 • VertexSound
 • Button Improvements 
 • VertexUI
 • Tension 2D (New renderer)
 • Complete overhaul of how renderering is set up

Tension2D Features:

 • Layers will be introduced in the Tension2D renderer (Completed)
 • Gameobjects will need to render their own children in the Tension2D renderer 
 • Gameobjects will update their children instead of the asset manager
 • Render Textures introduced
 • Auto detect Transparency in textures
 • 2D Lighting 
 • Depth testing (Completed)
 • Improved transparency renderering (Completed)


Changes to come:
 • Materials will be changed to components & will be removed from base gameobjects
 • Renderers can be selected in the application header (Vertex2D & Tension2D)
 • Application will contain a Settings system that will set renderers & unique settings 
 for the AssetManager instead of having to change it manually in each manager


Major Features To Come:

 1. complete
 2. Scene Manager window automation 
 3. Physics Engine Simulations (Almost Complete)
 4. Vertex Canvas (UI Editor)
 5. OpenAL Intergration
 6. Complete
 7. Particle System
 8. (WIP) Editor Logger (Allow the Editor to write to disk objects new locations)
 9. Vertex2D Sprite Batch Support
 10. Animation Editor (Subject To Change)
 11. PlayerController Component
 12. 2D Light Support
 13. AI Agent Comonent (Subject To Change)
 14. Complete
 15. Scene Manager singleton conversion.
 16. Complete
 17. Fix Collsion

Editor Changes To Come:

 1. Gizmos (almost complete)
 2. Transform Gizmos
 3. Click Select
 4. Selection Outline
 5. Scale Gizmos
 6. Rotation Gizmos (subject to change)
 
Tension 2D vs Vertex 2D Renderer
1) Tension has layers & auto layer sorting.
vertex does not.

2) Tension has depth testing. Vertex does not

3) Tension only sorts layers once to save performance. 
Sorting before every frame is drawn can also be turned on
in a boot up container
