#version 450

out vec4 color;

void main() {
  color = vec4(gl_FragCoord.z * 1000.);
}
