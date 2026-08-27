#version 330 core

in vec2 uv;
out vec4 fragColor;
uniform sampler2D canvasTexture;

void main() {
    // v invertido: Canvas linha 0 = topo, mas textura OpenGL v=0 = base
    fragColor = texture(canvasTexture, vec2(uv.x, 1.0 - uv.y));
}
