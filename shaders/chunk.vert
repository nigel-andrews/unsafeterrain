#version 450

#include "utils.glsl"

in vec4 position;

layout(binding = 0) uniform Data {
    FrameData frame;
};

void main() {
  gl_Position = frame.camera.view_proj * position;
}
