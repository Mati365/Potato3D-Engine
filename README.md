# Potato3D Engine #
**Potato3D Engine** is simple lightweight 3D Engine written in C++0x and OpenGL 4.x. 

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

## Screens:
![](doc/img.png)

## License:
The MIT License (MIT)

Copyright (c) 2015 Mateusz Bagiński

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



