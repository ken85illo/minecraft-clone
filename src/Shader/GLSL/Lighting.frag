#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightPos = vec3(0.0, 0.0, 0.0);
uniform sampler2D texture0;

void main() {
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 color = ambient + diffuse;
    vec3 min = vec3(1.25);

    if (length(color) > length(min)) {
        color = min;
    }

    vec4 result = vec4(color, 1.0) * texture(texture0, TexCoord);
    FragColor = result;
}
