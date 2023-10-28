#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "model.h"
#include "util/util.h"


typedef struct {
    mat4 model, veiw, projection;
    
    Model *model;
} Renderer;
