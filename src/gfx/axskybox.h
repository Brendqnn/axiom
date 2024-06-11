#ifndef AXSKYBOX_H
#define AXSKYBOX_H

#include "axvao.h"
#include "axvbo.h"
#include "axebo.h"
#include "axshader.h"
#include "axtexture.h"
#include "../util/axcamera.h"


typedef struct {
    AXVao vao;
    AXVbo vbo;
    AXEbo ebo;

    AXTexture cubemap;
    AXShader shader;
} AXSkybox;

AXSkybox ax_init_skybox(const char *skybox_faces[]);
void ax_render_skybox(AXSkybox skybox, AXCamera *camera);

#endif // AXSKYBOX_H
