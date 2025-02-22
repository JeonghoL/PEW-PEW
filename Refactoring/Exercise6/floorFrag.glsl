#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform bool useFloorTexture;
uniform sampler2D floorTexture;

void main()
{
    if (useFloorTexture) {
        FragColor = texture(floorTexture, TexCoords);
    } else {
        FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);  // 기본 파란색
    }
}