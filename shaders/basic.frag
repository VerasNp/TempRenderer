#version 330 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D uCanvas;

void main() {
    fragColor = texture(uCanvas, vec2(uv.x, 1.0 - uv.y));
}
