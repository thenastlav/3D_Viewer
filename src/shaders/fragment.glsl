#version 330 core
in vec3 color;
out vec4 FragColor;

// Line style uniforms;
uniform bool uDashed;
uniform float uDashSize;
uniform float uGapSize;

void main()
{
    if (uDashed) {
        float position = mod(gl_FragCoord.x, uDashSize * uGapSize);
        if (position > uDashSize) discard;
    }

    FragColor = vec4(color, 1.0f);
}
