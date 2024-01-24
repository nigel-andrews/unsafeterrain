#version 450

#include "utils.glsl"

in vec4 position;
in vec4 normal;

out vec4 v_normal;
out vec4 v_position;

layout(binding = 0) uniform Data {
    FrameData frame;
};

void main() {
  gl_Position = frame.camera.view_proj * position;
  v_normal = normal;
}
