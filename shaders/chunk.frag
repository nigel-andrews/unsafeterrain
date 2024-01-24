#version 450

#include "utils.glsl"

in vec4 v_position;
in vec4 v_normal;

out vec4 color;

layout(binding = 0) uniform Data {
    FrameData frame;
};

layout(binding = 1) buffer PointLights {
    PointLight point_lights[];
};

const vec3 ambient = vec3(0.0);

void main() {
  // color = vec4(gl_FragCoord.z * 1000.);
  // color = vec4(v_normal);
  vec3 normal = normalize(v_normal.xyz);
  vec3 acc = frame.sun_color * max(0.0, dot(frame.sun_dir, normal)) + ambient;

  vec3 in_position = v_position.xyz;
  vec3 in_color = vec3(1.0);

  for(uint i = 0; i != frame.point_light_count; ++i) {
      PointLight light = point_lights[i];
      const vec3 to_light = (light.position - in_position);
      const float dist = length(to_light);
      const vec3 light_vec = to_light / dist;

      const float NoL = dot(light_vec, normal);
      const float att = attenuation(dist, light.radius);
      if(NoL <= 0.0 || att <= 0.0f) {
          continue;
      }

      acc += light.color * (NoL * att);
  }

  color = vec4(in_color * acc, 1.0);
}
