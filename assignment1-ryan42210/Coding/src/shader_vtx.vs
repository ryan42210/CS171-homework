#version 330 core
layout (location = 0) in vec3 pos_local;
layout (location = 1) in vec3 normal_local;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 lightPos1_world;
uniform vec3 lightPos2_world;


out vec3 fragNormal_view;
out vec3 fragPos_view;

out vec3 lightPos1_view;
out vec3 lightPos2_view;

void main() {
    gl_Position = proj * view * model * vec4(pos_local, 1.0);
    fragNormal_view = vec3((transpose(inverse(view * model))) * vec4(normal_local, 0.0));
    fragPos_view = vec3(view * model * vec4(pos_local, 1.0f));
    lightPos1_view = vec3(view * vec4(lightPos1_world, 1.0));
    lightPos2_view = vec3(view * vec4(lightPos2_world, 1.0));
}
