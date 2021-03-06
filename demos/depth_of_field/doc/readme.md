# DEPTH OF FIELD

This example implements depth of field based on the depth of field article by 
Nathaniel Meyer, http://devmaster.net/posts/3021/shader-effects-depth-of-field.
In short, this depth of field shader applies a blur based on the distance of a 
fragment to the camera. There is a certain area where we do not blur at all 
(focus distance). 

We perform the following steps:

- render complete scene, store diffuse in a texture and save depth map as linear image
- apply vertical blur based on the distance following the circle of confusion blur radius (see link)
- apply horizontal blur based on the distance ... "" 
- the last pass interpolates between the blurred texture and the scene/diffuse texture 

<img src="http://upload.roxlu.com/server/php/files/dof.png">
