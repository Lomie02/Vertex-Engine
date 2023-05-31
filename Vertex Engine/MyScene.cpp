rence** to the name `FindProperty` uses. In this example, it's **_MyColor**.

![](Images/custom-material-inspector-node-settings-example.png)

The following image shows how the Inspector looks for the UI block in the code sample.

![](Images/custom-material-inspector-ui-block-example.png)

#### Implementing a foldout section

By default, UI blocks aren't nested in a foldout. The foldouts in other HDRP Material Inspectors use the `MaterialHeaderScope` class. This class specifies the name of the header and whether the section is expanded or not. For an example of how to implement a UI block in a foldout, see the following code sample:


```CSharp
class ColorUIBlock : MaterialUIBlock
{
    ExpandableBit   foldoutBit;

    MaterialProperty colorProperty;

    public ColorUIBlock(ExpandableBit expandableBit)
    {
        foldoutBit = expandableBit;
    }

    public override void LoadMaterialProperties()
    {
        colorProperty = FindProperty("_MyColor");
    }

    public override void OnGUI()
    {
        using (var header = new MaterialHeaderScope("Color Options", (uint)foldoutBit, materialEditor))
        {
            if (header.expanded)
            {
                materialEditor.ShaderProperty(colorProperty, "My Color");
            }
        }
    }
}
```


**Note**: To track whether the foldout is expanded or not, `MateralHeaderScope` uses an `ExpandableBit`. To assign the `ExpandableBit`, this UI block example has a constructor that takes an ExpandableBit as a parameter. Because Unity serializes the state of each foldout in Editor preferences, you should use the `User[0..19]` part of the ExpandableBit enum to avoid overlap with built-in reserved bits. For an example of how to do this, see the code sample in [Custom Lit Material Inspector](#custom-lit-material-inspector).

You can also hardcode the bit in a UI block but this isn't best practice especially if you intend to create a lot of UI blocks that multiple materials share.

The following image shows how the Inspector looks for the UI block in the above code sample.

![](Images/custom-material-inspector-ui-block-foldout-example.png)

	m_Button->material.baseTexture = ResourceManager::GetTexture("boy1");

If you need to access another UI block from your current UI block, the `parent` member gives you access to the list of UI blocks in the custom Material Inspector. You can use this to find the UI block and use the result to call a function or get a material property for example.


```CSharp
var surfaceBlock = parent.FirstOrDefault(b => b is SurfaceOptionUIBlock) as SurfaceOptionUIBlock;
```


**Note**: You can’t access the parent in the constructor of a UIBlock so be sure to access it either in `LoadMaterialProperties` or `OnGUI`.

## Examples

This section provides example implementations for the following custom Material Inspectors :

- [Lit](#custom-lit-material-inspector)
- [Unlit](#custom-unlit-material-inspector)
- [Decals](#custom-decal-material-inspector)
- [Bespoke](#bespoke-material-inspector)

### Custom Lit Material Inspector

For Lit Materials, the custom Material Inspector should inherit from `LightingShaderGraphGUI`. The `LightingShaderGraphGUI` represents any Shader Graph that uses lighting. For HDRP, this includes Lit, StackLit, Hair, Fabric, and Eye.

The `LightingShaderGraphGUI` class directly inherits from `HDShaderGUI` and overrides every function that renders the UI. This means that any class that inherits from `LightingShaderGraphGUI` already works correctly, all the new class needs to do is add/remove some UI blocks. For an example of this, see the following code snippet:

```CSharp
using UnityEditor.Rendering.HighDefinition;

public class LightingInspectorExample : LightingShaderGraphGUI
{
    public LightingInspectorExample()
    {
        // Remove the ShaderGraphUIBlock to avoid having duplicated properties in the UI.
        uiBlocks.RemoveAll(b => b is ShaderGraphUIBlock);

	m_Button->material.colour = glm::vec4(1,0,0,1);
	m_Manager.GiveWindow(m_Window);
	m_Object->transform.size = glm::vec2(1, 1);

	m_Object->transform.position.x = 0;
	m_Object->transform.position.y = 0;

	//m_Object2->transform.position.y = 20;
	//m_Object2->transform.position.x = 20;

	//m_Body->transform.position.x = 0;
	//m_Body->transform.position.y = 0;

	//m_Body->transform.size = glm::vec2(2, 2);

	m_Object2->transform.size = glm::vec2(2, 1);

	m_Button->transform.size.x = 5;
	m_Button->transform.size.y = 3;

	m_Button->text = "Play";

	m_Button->transform.position.x = 5.0f;
	m_Button->transform.position.y = 5.0f;

	m_MainCamera->transform.position.x = 10;
	m_MainCamera->transform.position.y = 10;

	m_MyText->transform.position.x = 10;
	m_MyText->transform.position.y = 10;

	glClearColor(0.2f, 0.2f, 0.2f, 0);

}
```


This code sample produces the following Inspector:

![](Images/custom-material-inspector-lit-example.png)

### Custom Unlit Material Inspector

For Unlit Materials, the custom Material Inspector should inherit from `UnlitShaderGraphGUI`.

The `UnlitShaderGraphGUI` class directly inherits from `HDShaderGUI` and overrides every function that renders the UI. This means that any class that inherits from `UnlitShaderGraphGUI` already works correctly, all the new class needs to do is add/remove some UI blocks. For an example of this, see the following code snippet:

```CSharp
using UnityEditor.Rendering.HighDefinition;

public class UnlitExampleGUI : UnlitShaderGraphGUI
{
    public UnlitExampleGUI()
    {
        // Remove the ShaderGraphUIBlock to avoid having duplicated properties in the UI.
        uiBlocks.RemoveAll(b => b is ShaderGraphUIBlock);

        // Insert the color block just after the Surface Option b