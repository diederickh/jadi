#include "BloomHelper.h"

BloomHelper::BloomHelper() {
}

BloomHelper::~BloomHelper() {
}

void BloomHelper::setup() {
  
  setupShaders();
  setupBuffers();

}

void BloomHelper::drawBloomableAreas(const float* pm, const float* vm, std::vector<Particle>& particles) {
  draw(pm, vm, particles, tex_bloom);
}

void BloomHelper::drawShaded(const float* pm, const float* vm, std::vector<Particle>& particles) {
 draw(pm, vm, particles, tex_shaded);
}

void BloomHelper::draw(const float* pm, const float* vm, std::vector<Particle>& particles, GLuint tex) {
  glEnable(GL_DEPTH_TEST);
  glBindVertexArray(vao);
  glUseProgram(prog);
  GLint upm = glGetUniformLocation(prog, "u_pm");
  GLint uvm = glGetUniformLocation(prog, "u_vm");
  GLint umm = glGetUniformLocation(prog, "u_mm");
  GLint utex = glGetUniformLocation(prog, "u_tex");
  glUniformMatrix4fv(upm, 1, GL_FALSE, pm);
  glUniformMatrix4fv(uvm, 1, GL_FALSE, vm);

  // set texture 
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(utex, 0);

  Mat4 mm;
  for(std::vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
    Particle& p = *it;
    mm.setPosition(p.position.x, p.position.y, p.position.z);
    mm.setScale(4.5);
    glUniformMatrix4fv(umm, 1, GL_FALSE, mm.getPtr());
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
  }
}

void BloomHelper::setupBuffers() {
  // Create VAO + VBO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  float* vertices; 
  num_vertices = load_obj_file("sphere.obj", &vertices, true, true);
  glBufferData(GL_ARRAY_BUFFER, num_vertices * 6 * sizeof(float), vertices, GL_STATIC_DRAW);
  delete[] vertices;

  glEnableVertexAttribArray(0); // pos
  glEnableVertexAttribArray(1); // normal
  glEnableVertexAttribArray(2); // tex
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)12);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)24);

  // Create texture
  Image img;
  img.load(to_data_path("bloom.png"));
  glGenTextures(1, &tex_bloom);
  glBindTexture(GL_TEXTURE_2D, tex_bloom);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixels());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  img.load(to_data_path("shaded.png"));
  glGenTextures(1, &tex_shaded);
  glBindTexture(GL_TEXTURE_2D, tex_shaded);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixels());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void BloomHelper::setupShaders() {
  // Used to render 
  prog = create_shader(BH_BASIC_VS, BH_BASIC_FS);
  glBindAttribLocation(prog, 0, "a_pos");
  glBindAttribLocation(prog, 1, "a_norm");
  glBindAttribLocation(prog, 2, "a_tex");
  glLinkProgram(prog);
}
