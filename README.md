# Potato3D Engine #
-------------------
**Potato3D Engine** is simple lightweight 3D Engine written in C++0x and OpenGL 4.x. 

-------------------
## What's done:
+ 	VBO/VAO
+ 	Render to texture/FBO
+ 	Custom matrix stack implementation
+	Matrix math
+ 	Bitmap fonts
+	Specular/Diffuse/Ambient Lighting
+	Materials
+	*.obj .mtl* mesh loading
+	Normal mapping

-------------------
## Usage:
```cpp
[...]
cam = &scene.createSceneNode<Camera>()
       .setPos( { 0.f, .5f, 1.f, 1.f })
       .setTarget( { 0.f, .5f, -2.f, 1.f });
scene.createSceneNode<AxisMesh>();
scene.createSceneNode<LightBatch>()
       .regObject(
       scene.createSceneNode<Light>()
               .setPos( { 0.f, 0.5, 0.f })
               .setSpecular( { 1.f, 1.f, 1.f, 1.f }, 1.f)
               .setDiffuse( { 1.f, 1.f, 1.f, 1.f, }, 1.f)
               );
scene.createSceneNode<Mesh>()
       .setShape(
       GlobalResourceManager::getInstance().getResource<Shape3D>(
               "mesh/truck/untitled.obj"))
       .getTransform()
       .mul(MatMatrix::scale( { .2f, .2f, .2f }))
       .mul(MatMatrix::translate( { 0.f, 0.5f, -10.f }));

scene.createSceneNode<Text>()
       .setText("Test czcionek!")
       .setPos( { 0.f, 0.5f, 0.f })
       .setSize(4.f);

fbo = dynamic_cast<RenderQuad*>(
       &scene.createSceneNode<RenderQuad>()
               .setSize(scene.getRenderResolution())
               .setShaderParam("blur", { 0.f }, GL_FLOAT));
[...]
scene.draw();
```

-------------------
## Screens:
![](doc/img.png)



