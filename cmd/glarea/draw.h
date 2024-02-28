typedef struct {
  float position[3];
  float color[3];
} Vertex;

static void draw_mvp_reset(float*);
static void draw_buffers_init(GLuint, GLuint);

void draw_render();
void draw_shutdown();
void draw_init();
