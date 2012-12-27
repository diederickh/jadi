#include "Demo.h"

Demo::Demo() {
}

Demo::~Demo() {
}

void Demo::setup() {
  cam.setupArcball(1024, 768);
  cam.setPerspective(60.0f, 4.0f/3.0f, 0.1, 350.0f);
  cam.setPosition(0,50,90);

  setupShaders();
  setupBuffers();
  setupImages();
  
  glClearColor(.1f, .1f, .1f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

void Demo::update() {
}

void Demo::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  const float* pm = cam.pm().getPtr();
  const float* vm = cam.vm().getPtr();
  
  Mat4 mm;
  mm.identity();
  
  glUseProgram(scene_prog);
  
  glBindVertexArray(statue_vao);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  
  glUniformMatrix4fv(glGetUniformLocation(scene_prog, "u_pm"), 1, GL_FALSE, pm);
  glUniformMatrix4fv(glGetUniformLocation(scene_prog, "u_vm"), 1, GL_FALSE, vm);
  glUniformMatrix4fv(glGetUniformLocation(scene_prog, "u_mm"), 1, GL_FALSE, mm.getPtr());
  
  glUniform1i(glGetUniformLocation(scene_prog, "u_texture"), 0);
  
  glDrawArrays(GL_TRIANGLES, 0, num_vertices);
  
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  
  glUseProgram(0);
}

void Demo::setupShaders() {
  scene_prog = createProgram(STATUE_SCENE_VS, STATUE_SCENE_FS);
  
  std::cout << scene_prog << std::endl;
}

GLuint Demo::createProgram(const char* vs, const char* fs) {
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

void Demo::setupBuffers() {
  glGenVertexArrays(1, &statue_vao);  // create vao
  glBindVertexArray(statue_vao); // bind vao
  glGenBuffers(1, &statue_vbo);  // create 1 interleaved vbo (vertex, normal, texcoords)
  glBindBuffer(GL_ARRAY_BUFFER, statue_vbo);  // bind vbo

  float* vertices;
  num_vertices = load_obj_file("statue.obj", &vertices, true, true);
  
  float num_floats = 8;
  glBufferData(GL_ARRAY_BUFFER, num_vertices * num_floats * sizeof(float), vertices, GL_STATIC_DRAW);
  delete[] vertices;
  
  GLint pos = glGetAttribLocation(scene_prog, "a_pos");
  GLint norm = glGetAttribLocation(scene_prog, "a_norm");
  GLint texCoord = glGetAttribLocation(scene_prog, "a_texcoord");
  
  // xyz xyz st
  glEnableVertexAttribArray(pos);
  glEnableVertexAttribArray(norm);
  glEnableVertexAttribArray(texCoord);
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(float)*num_floats, (GLvoid*)0);
  glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, sizeof(float)*num_floats, (GLvoid*)12); // 12 bytes in (3 * 4 byte floats)
  glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, sizeof(float)*num_floats, (GLvoid*)24); // 24 bytes in (6 * 4 byte floats)
 
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Demo::setupImages() {
  Image img;
  img.load(to_data_path("statue_diffuse.jpg"));
  
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.getWidth(), img.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.getPixels());
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Demo::onMouseDown(int x, int y, int button) {
  cam.onMouseDown(x, y);
}

void Demo::onMouseUp(int x, int y, int button) {
}

void Demo::onMouseDrag(int x, int y, int dx, int dy, int button) {
  cam.onMouseDrag(x,y);
}

void Demo::onMouseMove(int x, int y) {
}

void Demo::onChar(int ch) {
}

void Demo::onKeyDown(int key) {
}

void Demo::onKeyUp(int key) {
}

void Demo::onWindowResize(int w, int h) {
}

void Demo::onWindowClose() {
}
