#version 330 core

in vec2 uv;
out vec4 fragColor;
uniform sampler2D canvasTexture;

void main() {
    fragColor = texture(canvasTexture, vec2(uv.x, 1.0 - uv.y));
}
