
#define pi 3.1415926535897932384626433832795
#define pi2 pi * 2.0

GLfloat *gen_rectangle(GLfloat x, GLfloat y, GLfloat z, GLfloat w1,
                       GLfloat w2) {
  GLfloat *result = new GLfloat[6 * 3];

  result[0] = x + w1;
  result[1] = y;
  result[2] = z + w2;

  result[3] = x + w1;
  result[4] = y;
  result[5] = z - w2;

  result[6] = x - w1;
  result[7] = y;
  result[8] = z + w2;

  result[9] = x - w1;
  result[10] = y;
  result[11] = z - w2;

  result[12] = x + w1;
  result[13] = y;
  result[14] = z - w2;

  result[15] = x - w1;
  result[16] = y;
  result[17] = z + w2;

  return result;
}

GLfloat *gen_cylinder(GLfloat x, GLfloat y, GLfloat z, GLfloat h, GLfloat r,
                      size_t resolution) {
  GLfloat *result = new GLfloat[resolution * 18];

  for (size_t i = 0; i < resolution * 18; i += 18) {
    size_t f = i / 18;
    result[i + 0] = x + (r * cos(f * pi2 / resolution));
    result[i + 1] = y;
    result[i + 2] = z + (r * sin(f * pi2 / resolution));

    result[i + 3] = x + (r * cos((f + 1ll) * pi2 / resolution));
    result[i + 4] = y;
    result[i + 5] = z + (r * sin((f + 1ll) * pi2 / resolution));

    result[i + 6] = x + (r * cos(f * pi2 / resolution));
    result[i + 7] = y + h;
    result[i + 8] = z + (r * sin(f * pi2 / resolution));

    result[i + 9] = x + (r * cos(f * pi2 / resolution));
    result[i + 10] = y + h;
    result[i + 11] = z + (r * sin(f * pi2 / resolution));

    result[i + 12] = x + (r * cos((f + 1ll) * pi2 / resolution));
    result[i + 13] = y + h;
    result[i + 14] = z + (r * sin((f + 1ll) * pi2 / resolution));

    result[i + 15] = x + (r * cos((f + 1ll) * pi2 / resolution));
    result[i + 16] = y;
    result[i + 17] = z + (r * sin((f + 1ll) * pi2 / resolution));
  }

  return result;
}

GLfloat *gen_cone(GLfloat x, GLfloat y, GLfloat z, GLfloat h, GLfloat r,
                  size_t resolution) {
  GLfloat *result = new GLfloat[resolution * 9];

  for (size_t i = 0; i < resolution * 9; i += 9) {
    size_t f = i / 9;
    result[i + 0] = x + (r * cos(f * pi2 / resolution));
    result[i + 1] = y;
    result[i + 2] = z + (r * sin(f * pi2 / resolution));

    result[i + 3] = x;
    result[i + 4] = y + h;
    result[i + 5] = z;

    result[i + 6] = x + (r * cos((f + 1ll) * pi2 / resolution));
    result[i + 7] = y;
    result[i + 8] = z + (r * sin((f + 1ll) * pi2 / resolution));
  }

  return result;
}

GLfloat *gen_umbrella(GLfloat x, GLfloat y, GLfloat z, GLfloat h, GLfloat r,
                      size_t resolution) {
  GLfloat *result = new GLfloat[resolution * 9];

  for (size_t i = 0; i < resolution * 9; i += 9) {
    size_t f = i / 9;
    result[i + 0] = x + (r * cos(f * pi2 / resolution));
    result[i + 1] = y;
    result[i + 2] = z + (r * sin(f * pi2 / resolution));

    result[i + 3] = x;
    result[i + 4] = y - h;
    result[i + 5] = z;

    result[i + 6] = x + (r * cos((f + 1ll) * pi2 / resolution));
    result[i + 7] = y;
    result[i + 8] = z + (r * sin((f + 1ll) * pi2 / resolution));
  }

  return result;
}

GLfloat *cal_normal(GLfloat *vertexs, size_t count_of_vertex) {
  GLfloat* result = new GLfloat[count_of_vertex * 3];

  for (size_t i = 0; i < count_of_vertex; i += 3*3) {
    auto a = glm::vec3(vertexs[i + 0], vertexs[i + 1], vertexs[i + 2]);
    auto b = glm::vec3(vertexs[i + 3], vertexs[i + 4], vertexs[i + 5]);
    auto c = glm::vec3(vertexs[i + 6], vertexs[i + 7], vertexs[i + 8]);

    auto ab = b - a;
    auto ac = c - a;

    auto n = normalize(cross(ab, ac));

    result[i + 0] = result[i + 3] = result[i + 6] = n.x;
    result[i + 1] = result[i + 4] = result[i + 7] = n.y;
    result[i + 2] = result[i + 5] = result[i + 8] = n.z;
  }

  return result;
}

GLfloat *cal_normalc(GLfloat *vertexs, size_t count_of_vertex) {
  GLfloat* result = new GLfloat[count_of_vertex * 3];

  for (size_t i = 0; i < count_of_vertex; i += 3*3) {
    auto a = glm::vec3(vertexs[i + 0], vertexs[i + 1], vertexs[i + 2]);
    auto b = glm::vec3(vertexs[i + 3], vertexs[i + 4], vertexs[i + 5]);
    auto c = glm::vec3(vertexs[i + 6], vertexs[i + 7], vertexs[i + 8]);

    auto ab = b - a;
    auto ac = c - a;

    auto n = normalize(cross(ab, ac));

    result[i + 0] = n.x;
    result[i + 1] = n.y;
    result[i + 2] = n.z;
    
    result[i + 3] = n.x;
    result[i + 4] = n.y;
    result[i + 5] = n.z;
    
    result[i + 6] = -n.x;
    result[i + 7] = -n.y;
    result[i + 8] = -n.z;
  }

  return result;
}

GLfloat *dup3(GLfloat a, GLfloat b, GLfloat c, size_t sz) {
  GLfloat *result = new GLfloat[sz * 3];

  for (int i = 0; i < sz * 3; i += 3) {
    result[i + 0] = a;
    result[i + 1] = b;
    result[i + 2] = c;
  }

  return result;
}

static GLfloat *ground = gen_rectangle(0, 0, 0, 1000.0f, 1000.f);
static GLfloat *ground_color = dup3(0.f, 0.5f, 0.1f, 18);
static GLfloat *ground_n = cal_normal(ground, 18);

static GLfloat *cloud = gen_rectangle(0.0, 0.0, 0.0, 5, 10);
static GLfloat *cloud_color = dup3(1, 1, 1, 18);
static GLfloat *cloud_n = cal_normal(cloud, 18);
static glm::mat4 cloud_pos[] = {
  translate(mat4(), glm::vec3(5, 43, 0)),
  translate(mat4(), glm::vec3(10.2, 40, 20)),
  translate(mat4(), glm::vec3(-5, 40.5, -25.4)),
  translate(mat4(), glm::vec3(-15, 45, -10)),
  translate(mat4(), glm::vec3(-30, 42, 10)),
};

static GLfloat *tree_trunk = gen_cylinder(0.0, 0.0, 0.0, 5.f, 1.f, 36);
static GLfloat *tree_trunk_color = dup3(0.505, 0.337, 0.0f, 36 * 6);
static GLfloat *tree_trunk_n = cal_normalc(tree_trunk, 36 * 6);
static GLfloat *tree_leaf = gen_cone(0.0, 5.f, 0.0, 8.f, 3.f, 36);
static GLfloat *tree_leaf_color = dup3(0.1f, 0.8, 0.3f, 36 * 3);
static GLfloat *tree_leaf_n = cal_normal(tree_leaf, 36 * 3);
static GLfloat *tree_leaf2 = gen_cone(0.0, 8.f, 0.0, 6.f, 2.5f, 36);
static GLfloat *tree_leaf2_color = dup3(0.4f, 0.8, 0.3f, 36 * 3);
static GLfloat *tree_leaf2_n = cal_normal(tree_leaf2, 36 * 3);
static vec3 tree_pos[] = {
  glm::vec3(15, 0, 15),
  glm::vec3(21, -2, 10),
  glm::vec3(21, -2, 17),
  glm::vec3(-10, 0, -10),
  glm::vec3(-4, -1, -8),
  glm::vec3(-32.3379, -2.0517, 0.160711),
  glm::vec3(-34.171, -2.4587, 7.315),
  glm::vec3(-29.1132, -1.884, 14.8601),
  glm::vec3(-23.9865, -1.2619, 20.5402),
  glm::vec3(-18.6205, -0.5439, 23.1598),
  glm::vec3(-13.5307, -0.75477, 20.2612),
  glm::vec3(31.1066, -0.3551, -27.6544),
  glm::vec3(27.9734, -0.1658, -33.8723),
  glm::vec3(16.8891, -0.233, -33.9048),
  glm::vec3(11.0246, -1.6686, -26.8673),
  glm::vec3(10.207, -2.9304, -33.2954),
  glm::vec3(10.5234, -1.4443, -26.2425),
};


static GLfloat *rocket_body = gen_cylinder(0.0, 0.0, 0.0, 10, 1, 36);
static GLfloat *rocket_body_color = dup3(0.5, 0.5, 0.5, 36 * 6);
static GLfloat *rocket_body_n = cal_normalc(rocket_body, 36 * 6);

static GLfloat rocket_side[] = {
    1.0f,  0.0f, 0.0f, 2.5f,  0.0f, 0.0f, 1.0f,  2.5f, 0.0f,

    -1.0f, 0.0f, 0.0f, -2.5f, 0.0f, 0.0f, -1.0f, 2.5f, 0.0f,
};
static GLfloat *rocket_side_color = dup3(0.9, 0.1, 0.1, 18);
static GLfloat *rocket_side_n = cal_normal(rocket_side, 6);

static GLfloat *rocket_head = gen_cone(0, 10, 0, 2, 1, 36);
static GLfloat *rocket_head_color = dup3(0.9, 0.1, 0.1, 36 * 3);
static GLfloat *rocket_head_n = cal_normal(rocket_head, 36 * 3);

static GLfloat *rocket_bottom = gen_cone(0, 0, 0, 0, 1, 36);
static GLfloat *rocket_bottom_color = dup3(0.5, 0.5, 0.5, 36 * 3);
static GLfloat *rocket_bottom_n = cal_normal(rocket_bottom, 36 * 3);

static const GLfloat parachute_line[] = {
    0.0f,  0.0f,  0.0f,  -2.825f, -2.0f, -2.825f,
    0.0f,  0.0f,  0.0f,  -2.825f, -2.0f, 2.825f,
    0.0f,  0.0f,  0.0f,  2.825f, -2.0f, 2.825f,
    0.0f,  0.0f,  0.0f,  2.825f,  -2.0f, -2.825f,

    0.0f,  0.0f,  0.0f,  0.0f,  -2.0f, 4.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  -2.0f, -4.0f,
    0.0f,  0.0f,  0.0f,  4.0f,  -2.0f, 0.0f,
    0.0f,  0.0f,  0.0f,  -4.0f, -2.0f, 0.0f,
};
static GLfloat *parachute_line_color = dup3(1.0, 1.0, 1.0, 16);

static GLfloat *parachute = gen_umbrella(0, -2.0, 0, 2.5, 4.0, 8);
static GLfloat *parachute_color = dup3(0.392, 0.219, 0.784, 24);
static GLfloat *parachute_n = cal_normal(parachute, 24);

class graphic_buffer {
  GLfloat *data;
  size_t sz;

public:
  graphic_buffer(GLfloat *data, size_t size) : data(data), sz(size) {}
  GLuint gen() {
    GLuint ttid;
    glGenBuffers(1, &ttid);
    glBindBuffer(GL_ARRAY_BUFFER, ttid);
    glBufferData(GL_ARRAY_BUFFER, sz, data, GL_STATIC_DRAW);
    return ttid;
  }
};

class graphic_object {
  graphic_buffer data_buffer;
  graphic_buffer color_buffer;
  graphic_buffer normal_buffer;
  GLuint ver, col, normal;
  size_t tri;
  GLenum type;

public:
  graphic_object(GLfloat *data_buffer, size_t db_sz, GLfloat *color_buffer,
                 size_t cb_sz, GLfloat *normal_buffer, size_t tri, GLenum type = GL_TRIANGLES)
      : data_buffer(data_buffer, db_sz), color_buffer(color_buffer, cb_sz), normal_buffer(normal_buffer, db_sz),
        tri(tri), type(type) {
    ver = this->data_buffer.gen();
    col = this->color_buffer.gen();
    if (normal_buffer != nullptr)
      normal = this->normal_buffer.gen();
    else
      normal = -1;
  }

  void draw() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, ver);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, col);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    
    if (normal >= 0) {
      glEnableVertexAttribArray(2);
      glBindBuffer(GL_ARRAY_BUFFER, normal);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }

    glDrawArrays(type, 0, tri);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }

  void dispose() {
    glDeleteBuffers(1, &ver);
    glDeleteBuffers(1, &col);
    glDeleteBuffers(1, &normal);
  }

  GLuint buffer() const { return ver; }
  GLuint color() const { return ver; }
};
