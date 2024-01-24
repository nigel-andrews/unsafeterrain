#version 450

in vec4 position;

layout(location = 0) uniform uint buffer_width;
layout(location = 1) uniform ivec2 offset;

void main() {
  gl_Position = position;
}
