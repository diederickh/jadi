#include "Bloom.h"

Bloom::Bloom()
{
}

Bloom::~Bloom() {
}

void Bloom::debugDraw() {
  /*
  drawTexture(tex_occlusion, 0, 0, 512, 384);
  drawTexture(tex_rays, 0, 384, 512, 384);
  drawTexture(tex_shaded, 512, 0, 512, 384);

  // here we merge both
  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);
  glBlendFunc(GL_ONE, GL_ONE);
  drawTexture(tex_occlusion, 512, 384, 512, 384);
  drawTexture(tex_rays, 512, 384, 512, 384);
  drawTexture(tex_shaded, 512, 384, 512, 384);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  */
}

void Bloom::drawTexture(GLuint tex, int x, int y, int w, int h) {
  /*
  // setup framebuffer/viewport/program/vao
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK_LEFT);
  glUseProgram(image_prog);
  glViewport(x, y, w, h);
  glBindVertexArray(vao);

  // set texture
  GLint utex = glGetUniformLocation(image_prog, "u_tex");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(utex, 0);

  // and draw!
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  // reset
  glViewport(0,0,1024, 768);
  */
}

void Bloom::setup(int w, int h) {
  /*
  fbo_w = w;
  fbo_h = h;
  setupShaders();
  setupBuffers();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  */
}

void Bloom::setupShaders() {
  /*
  // Create debug image drawer
  image_prog = createShader(LR_SHARED_VS, LR_IMAGE_FS); 
  glBindAttribLocation(image_prog, 0, "a_pos");
  glBindAttribLocation(image_prog, 1, "a_tex"); 
  glLinkProgram(image_prog);  

  // Create rays shader
  rays_prog = createShader(LR_SHARED_VS, LR_RAYS_FS); 
  glBindAttribLocation(rays_prog, 0, "a_pos");
  glBindAttribLocation(rays_prog, 1, "a_tex"); 
  glLinkProgram(rays_prog);  


  // Get uniforms
  glUseProgram(rays_prog);
  uexposure = glGetUniformLocation(rays_prog, "u_exposure");
  utex = glGetUniformLocation(rays_prog, "u_tex");
  udecay = glGetUniformLocation(rays_prog, "u_decay");
  udensity = glGetUniformLocation(rays_prog, "u_density");
  uweight = glGetUniformLocation(rays_prog, "u_weight");
  */
}

GLuint Bloom::createShader(const char* vs, const char* fs) {
  /*
  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vert_id, 1, &vs, NULL);
  glShaderSource(frag_id, 1, &fs, NULL);
  glCompileShader(vert_id); eglGetShaderInfoLog(vert_id);
  glCompileShader(frag_id); eglGetShaderInfoLog(frag_id);

  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert_id);
  glAttachShader(prog, frag_id);

  return prog;
  */
  return 0;
}

void Bloom::setupBuffers() {
  /*
  // SETUP FB
  // -------------------------------------------
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  
  // Depth buffer
  glGenRenderbuffers(1, &depth_rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, fbo_w, fbo_h);
  glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rbo);

  // Render occluded scene to texture
  glGenTextures(1, &tex_occlusion);
  glBindTexture(GL_TEXTURE_2D, tex_occlusion);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbo_w, fbo_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_occlusion, 0);

  // Occluded scene is used to create rays which are rendered into tex_rays
  glGenTextures(1, &tex_rays);
  glBindTexture(GL_TEXTURE_2D, tex_rays);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbo_w, fbo_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex_rays, 0);

  // Shaded scene (not really necessary but handy to debug draw all stages)
  glGenTextures(1, &tex_shaded);
  glBindTexture(GL_TEXTURE_2D, tex_shaded);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbo_w, fbo_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, tex_shaded, 0);

  eglCheckFramebufferStatus();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK_LEFT);


  // SETUP VAO/VBO (fullscreen)
  // -------------------------------------------
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  GLfloat verts[] = {
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f, 
    1.0f, 1.0f, 1.0f, 1.0f,

    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 1.0f
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0); // pos
  glEnableVertexAttribArray(1); // tex
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)8);
  */
}


