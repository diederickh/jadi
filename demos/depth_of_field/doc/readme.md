# DEPTH OF FIELD

This example implements depth of field based on the depth of field article by 
Nathaniel Meyer, http://devmaster.net/posts/3021/shader-effects-depth-of-field.
In short, this depth of field shader applies a blur based on the distance of a 
fragment to the camera. There is a certain area where we do not blur at all 
(focus distance). 

We perform the following steps:
1) render complete scene, store diffuse in a texture and save depth map as linear image
2) apply vertical blur based on the distance following the circle of confusion blur radius (see link)
3) apply horizontal blur based on the distance ... ""

