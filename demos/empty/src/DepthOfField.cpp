#include "DepthOfField.h"

DepthOfField::DepthOfField() {
}

DepthOfField::~DepthOfField() {
}

void DepthOfField::setup(int w, int h) {
  fbo_w = w;
  fbo_h = h;
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  setupShaders();
  setupBuffers();
  setupFBO();
}

void DepthOfField::update() {
}

void DepthOfField::draw(const float* pm, const float* vm, const float* nm, std::vector<Particle>& particles) {
  glUseProgram(scene_prog);
  glBindVertexArray(vao);

  glUniformMatrix4fv(glGetUniformLocation(scene_prog, "u_pm"), 1, GL_FALSE, pm);
  glUniformMatrix4fv(glGetUniformLocation(scene_prog, "u_vm"), 1, GL_FALSE, vm);

  for(std::vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
    Particle& p = *it;
    mm.setPosition(p.position.x, p.position.y, p.position.z);
    glUniformMatrix4fv(glGetUniformLocation(scene_prog, "u_mm"), 1, GL_FALSE, mm.getPtr());
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
  }
}

void DepthOfField::debugDraw() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK_LEFT);
  drawTexture(image_prog, scene_tex, 0,384,512,384);
  drawTexture(debug_depth_prog, depth_tex, 0,0,512,384);
  drawTexture(image_prog, linear_depth_tex, 512, 0, 512, 384);
  drawTexture(image_prog, blur1_tex, 512, 384, 512, 384);
}


void DepthOfField::beginScenePass() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  GLenum bufs[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT2};
  glDrawBuffers(2, bufs);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(scene_prog);
  glViewport(0,0,fbo_w, fbo_h);
}

void DepthOfField::endScenePass() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK_LEFT);
}

void DepthOfField::applyDOF() {
  glDepthMask(GL_FALSE);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  glUseProgram(blur0_prog);
  glBindVertexArray(image_vao); 
  glViewport(0, 0, fbo_w, fbo_h);

  // first blur pass
  // ----------------
  glDrawBuffer(GL_COLOR_ATTACHMENT1); // blur0_tex
  //glClear(GL_COLOR_BUFFER_BIT); // not necessary (?)
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, scene_tex);
  glUniform1i(glGetUniformLocation(blur0_prog, "u_scene_tex"), 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, linear_depth_tex);
  glUniform1i(glGetUniformLocation(blur0_prog, "u_depth_tex"), 1);
  
  float t = millis() * 0.01;
  glUniform1f(glGetUniformLocation(blur0_prog, "u_time"), t);
  glUniform1i(glGetUniformLocation(blur0_prog, "u_mode"), 0);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // second blur pass
  // ----------------
  glDrawBuffer(GL_COLOR_ATTACHMENT3);
  glBindTexture(GL_TEXTURE_2D, blur0_tex);
  glUniform1i(glGetUniformLocation(blur0_prog, "u_scene_tex"), 0);
  glUniform1i(glGetUniformLocation(blur0_prog, "u_mode"), 1);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDepthMask(GL_TRUE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK_LEFT);
}

void DepthOfField::setupFBO() {
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

  // Depth buffer texture
  glGenTextures(1, &depth_tex);
  glBindTexture(GL_TEXTURE_2D, depth_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, fbo_w, fbo_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex, 0);

  // Scene texture
  glGenTextures(1, &scene_tex);
  glBindTexture(GL_TEXTURE_2D, scene_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbo_w, fbo_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scene_tex, 0);

  // H-blur texture
  glGenTextures(1, &blur0_tex);
  glBindTexture(GL_TEXTURE_2D, blur0_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo_w, fbo_h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, blur0_tex, 0);

  // Linear depth buffer 
  glGenTextures(1, &linear_depth_tex);
  glBindTexture(GL_TEXTURE_2D, linear_depth_tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbo_w, fbo_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, linear_depth_tex, 0);

  // V-blur texture
  glGenTextures(1, &blur1_tex);
  glBindTexture(GL_TEXTURE_2D, blur1_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo_w, fbo_h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, blur1_tex, 0);

  eglCheckFramebufferStatus();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthOfField::setupBuffers() {
  // VAO + VBO for debug.
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  float* vertices;
  num_vertices = load_obj_file("sphere.obj", &vertices, true);
  glBufferData(GL_ARRAY_BUFFER,  num_vertices * 6 * sizeof(float), vertices, GL_STATIC_DRAW);
  delete[] vertices;
    
  GLint pos = glGetAttribLocation(scene_prog, "a_pos");
  GLint norm = glGetAttribLocation(scene_prog, "a_norm");
  glEnableVertexAttribArray(pos);
  glEnableVertexAttribArray(norm);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)0);
  glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)12);

  // VAO + VBO for image debug drawer
  glGenVertexArrays(1, &image_vao);
  glBindVertexArray(image_vao);
  glGenBuffers(1, &image_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, image_vbo);
  
  float s = 1.0f;
  float img_verts[] = {
    -s, -s, 0.0f, 0.0f,
    s, -s, 1.0f, 0.0f,
    s, s, 1.0f, 1.0f,

    -s, -s, 0.0f, 0.0f,
    s, s, 1.0f, 1.0f,
    -s, s, 0.0f, 1.0f
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(img_verts), img_verts, GL_STATIC_DRAW);
  pos = glGetAttribLocation(image_prog, "a_pos");
  GLint tex = glGetAttribLocation(image_prog, "a_tex");
  glEnableVertexAttribArray(pos);
  glEnableVertexAttribArray(tex);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)0);
  glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)8);
}

void DepthOfField::setupShaders() {
  debug_depth_prog = createProgram(DOF_IMAGE_VS, DOF_DEBUG_DEPTH_IMAGE_FS); // debug: draw depth texture with gamma correction
  image_prog = createProgram(DOF_IMAGE_VS, DOF_IMAGE_FS); // debug: draw an image quad, just draw a texture
  scene_prog = createProgram(DOF_SCENE_VS, DOF_SCENE_FS); // dof step 0: renders the scene + writes linear depth 
  blur0_prog = createProgram(DOF_BLUR0_VS, DOF_BLUR0_FS); // dof step 1: renders the scene 
}

GLuint DepthOfField::createProgram(const char* vs, const char* fs) {
  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vert_id, 1, &vs, NULL);
  glShaderSource(frag_id, 1, &fs, NULL);
  glCompileShader(vert_id); eglGetShaderInfoLog(vert_id);
  glCompileShader(frag_id); eglGetShaderInfoLog(frag_id);
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert_id);
  glAttachShader(prog, frag_id);
  glLinkProgram(prog);
  return prog;
}

GLuint DepthOfField::createTexture(int w, int h, GLenum iformat, GLenum eformat) {
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, iformat, w, h, 0, eformat, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  return tex;
}

void DepthOfField::drawTexture(GLuint prog, GLuint tex, int x, int y, int w, int h) {
  glBindVertexArray(image_vao);
  glUseProgram(prog);
  
  glViewport(x,y,w,h); 

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(image_prog, "u_tex"), 0);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  glViewport(0,0,fbo_w, fbo_h);
}
