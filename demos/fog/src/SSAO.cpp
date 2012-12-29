#include "SSAO.h"

SSAO::SSAO() {
}

SSAO::~SSAO() {
}

void SSAO::setup(int w, int h) {
  fbo_w = w;
  fbo_h = h;
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  setupShaders();
  setupBuffers();
  setupFBO();
  loadImages();
  
  mm.identity();
  mm.scale(0.8f);
  mm.translate(0.0f, -50.0f, 0.0f);
}

void SSAO::update() {
}

void SSAO::draw(const float* pm, const float* vm, const float* nm) {
  glUseProgram(scene_prog);

  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, statue_tex);
  
  glBindVertexArray(vao);

  glUniformMatrix4fv(glGetUniformLocation(scene_prog, "u_pm"), 1, GL_FALSE, pm);
  glUniformMatrix4fv(glGetUniformLocation(scene_prog, "u_vm"), 1, GL_FALSE, vm);
  glUniformMatrix4fv(glGetUniformLocation(scene_prog, "u_mm"), 1, GL_FALSE, mm.getPtr());
  glUniform1i(glGetUniformLocation(scene_prog, "u_diffuseTexture"), 0);
  
  glDrawArrays(GL_TRIANGLES, 0, num_vertices);

  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
  glBindVertexArray(0);
}

void SSAO::debugDraw() {
  glDepthMask(GL_TRUE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK_LEFT);
  drawTexture(image_prog, position_tex, 0,384,512,384);
  drawTexture(depth_prog, depth_tex, 0,0,512,384);
  drawTexture(image_prog, linear_depth_tex, 512, 0, 512, 384);
  drawTexture(image_prog, scene_tex, 512, 384, 512, 384);
  
  glViewport(0,0,fbo_w, fbo_h);
}

void SSAO::beginScenePass() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  GLenum bufs[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
  glDrawBuffers(3, bufs);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(scene_prog);
  glViewport(0,0,fbo_w, fbo_h);
}

void SSAO::endScenePass() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK_LEFT);
}

void SSAO::applySSAO() {
  glDepthMask(GL_FALSE);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  GLenum bufs[] = {GL_COLOR_ATTACHMENT3};
  glDrawBuffers(1, bufs);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(image_vao);
  glUseProgram(ssao_prog);
  glViewport(0, 0, fbo_w, fbo_h);
  
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, linear_depth_tex);
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, position_tex);
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, random_tex);
  glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, albedo_tex);
  
  glUniform1i(glGetUniformLocation(ssao_prog, "u_normalAndDepthTex"), 0);
  glUniform1i(glGetUniformLocation(ssao_prog, "u_viewSpacePositionTex"), 1);
  glUniform1i(glGetUniformLocation(ssao_prog, "u_randomJitterTex"), 2);
  glUniform1i(glGetUniformLocation(ssao_prog, "u_albedoTex"), 3);

  glUniform1f(glGetUniformLocation(ssao_prog, "u_texelWidth"), 1.0f/(float)fbo_w);
  glUniform1f(glGetUniformLocation(ssao_prog, "u_texelHeight"), 1.0f/(float)fbo_h);
  
  glUniform1f(glGetUniformLocation(ssao_prog, "u_occluderBias"), 0.05f);
  glUniform1f(glGetUniformLocation(ssao_prog, "u_samplingRadius"), 20.0f);
  glUniform2f(glGetUniformLocation(ssao_prog, "u_attenuation"), 1.0f, 0.05f);
  glUniform1f(glGetUniformLocation(ssao_prog, "u_time"), (float)millis()/1000.0f);
  
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK_LEFT);
}

void SSAO::setupFBO() {
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

  // Depth buffer texture
  glGenTextures(1, &depth_tex);
  glBindTexture(GL_TEXTURE_2D, depth_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, fbo_w, fbo_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex, 0);

  // Diffuse texture
  glGenTextures(1, &position_tex);
  glBindTexture(GL_TEXTURE_2D, position_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, fbo_w, fbo_h, 0, GL_RGBA, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_tex, 0);

  // Linear depth buffer and normals
  glGenTextures(1, &linear_depth_tex);
  glBindTexture(GL_TEXTURE_2D, linear_depth_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, fbo_w, fbo_h, 0, GL_RGBA, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, linear_depth_tex, 0);

  // albedo texture (diffuse/textured, but unlit)
  glGenTextures(1, &albedo_tex);
  glBindTexture(GL_TEXTURE_2D, albedo_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbo_w, fbo_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedo_tex, 0);

  
  
  // final deferred render
  glGenTextures(1, &scene_tex);
  glBindTexture(GL_TEXTURE_2D, scene_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, fbo_w, fbo_h, 0, GL_RGBA, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, scene_tex, 0);

  eglCheckFramebufferStatus();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::setupBuffers() {
  // VAO + VBO for debug.
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  float* vertices;
  num_vertices = load_obj_file("statue.obj", &vertices, true, true); // vertices + normals + texcoords
  
  int num_floats = 8; // vec3 + vec3 + texcoord2
  glBufferData(GL_ARRAY_BUFFER,  num_vertices * num_floats * sizeof(float), vertices, GL_STATIC_DRAW);
  delete[] vertices;
    
  GLint pos = glGetAttribLocation(scene_prog, "a_pos");
  GLint norm = glGetAttribLocation(scene_prog, "a_norm");
  GLint texcoord = glGetAttribLocation(scene_prog, "a_tex");
  std::cout << texcoord << std::endl;
  
  glEnableVertexAttribArray(pos);
  glEnableVertexAttribArray(norm);
  glEnableVertexAttribArray(texcoord);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * num_floats, (GLvoid*)0);
  glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, sizeof(float) * num_floats, (GLvoid*)12);
  glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(float) * num_floats, (GLvoid*)24);

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

void SSAO::setupShaders() {
  depth_prog = createProgram(SSAO_IMAGE_VS, SSAO_DEPTH_FS); // debug: draw depth texture with gamma correction
  image_prog = createProgram(SSAO_IMAGE_VS, SSAO_IMAGE_FS); // debug: draw an image quad, just draw a texture
  scene_prog = createProgram(SSAO_SCENE_VS, SSAO_SCENE_FS); // dof step 0: renders the scene + writes linear depth
  ssao_prog = createProgram(SSAO_VS, SSAO_FS); // dof: applies box blur using 2 render passes
}

void SSAO::loadImages() {
  Image img;
  img.load(to_data_path("random.png"));
  random_tex = createTexture(img.getWidth(), img.getHeight(), GL_RGBA, GL_RGBA, img.getPixels());
  
  img.load(to_data_path("statue_diffuse.jpg"));
  statue_tex = createTexture(img.getWidth(), img.getHeight(), GL_RGB, GL_RGB, img.getPixels());
}

GLuint SSAO::createProgram(const char* vs, const char* fs) {
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

GLuint SSAO::createTexture(int w, int h, GLenum iformat, GLenum eformat, const unsigned char* data) {
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, iformat, w, h, 0, eformat, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  return tex;
}

void SSAO::drawTexture(GLuint prog, GLuint tex, int x, int y, int w, int h) {
  glBindVertexArray(image_vao);
  glUseProgram(prog);
  
  glViewport(x,y,w,h); 

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(image_prog, "u_tex"), 0);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  glViewport(0,0,fbo_w, fbo_h);
}
