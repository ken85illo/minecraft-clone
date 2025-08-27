#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform float fogMaxDist = 100.0;
uniform vec3 lightPos = vec3(0.0);
uniform float lightValue = 1.0;
uniform vec3 fogColor = vec3(1.0);
uniform sampler2D texture0;

void main() {
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * vec3(max(lightValue, 0.25));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(max(lightValue, 0.125));

    vec3 color = ambient + diffuse;
    vec3 max = vec3(1.125);

    if (length(color) > length(max)) {
        color = max;
    }

    float fogMinDist = fogMaxDist - 50.0;
    float dist = length(FragPos);
    float fogFactor = (fogMaxDist - dist) / (fogMaxDist - fogMinDist);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec4 result = mix(vec4(fogColor, 1.0), vec4(color, 1.0) * texture(texture0, TexCoord), fogFactor);
    FragColor = result;
}
