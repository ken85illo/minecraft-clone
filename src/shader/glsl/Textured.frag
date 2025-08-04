#version 460 core
out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 color = vec3(1.0, 1.0, 1.0);
uniform sampler2D texture0;

void main() {
    vec4 texColor = texture(texture0, TexCoord) * vec4(color, 1.0);

    if (texColor.a < 0.1)
        discard;

    FragColor = texColor;
}
