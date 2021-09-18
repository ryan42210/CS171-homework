#version 330 core
in vec3 fragNormal_view;
in vec3 fragPos_view;
in vec3 lightPos1_view;
in vec3 lightPos2_view;

out vec4 fragColor;

void main() {
    // basic color
    vec4 objColor = vec4(0.6f, 0.6f, 0.6f, 1.0f);
    vec3 lightcolor1 = vec3(0.8, 0.7, 0.4);
    vec3 lightcolor2 = vec3(0.1, 0.1, 0.6);
    float trans = 1.0;
    // ambient
    float ambt_factor1 = 0.2;
    // normal and light info
    vec3 normal_n = normalize(fragNormal_view);
    vec3 lightDir1_view = vec3(normalize(lightPos1_view - fragPos_view).xyz);
    // diffuse
    float diffuse_factor1 = max(dot(normal_n, lightDir1_view) , 0.0);
    // specular
    float specStrength = 0.8;
    vec3 reflectDir1_view = reflect(-lightDir1_view, normal_n);
    float spec_factor1 = specStrength * pow(max(dot(reflectDir1_view, normalize(-fragPos_view)), 0.0), 32);


    // ambient
    float ambt_factor2 = 0.2;

    vec3 lightDir2_view = vec3(normalize(lightPos2_view - fragPos_view).xyz);
    // diffuse
    float diffuse_factor2 = max(dot(normal_n, lightDir2_view) , 0.0);
    // specular
    vec3 reflectDir2_view = reflect(-lightDir2_view, normal_n);
    float spec_factor2 = specStrength * pow(max(dot(reflectDir2_view, normalize(-fragPos_view)), 0.0), 32);

    fragColor = (ambt_factor1 + diffuse_factor1 + spec_factor1) * vec4(lightcolor1, trans) * objColor + (ambt_factor2 + diffuse_factor2 + spec_factor2) * vec4(lightcolor2, trans) * objColor;
}