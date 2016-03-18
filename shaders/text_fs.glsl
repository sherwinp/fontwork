#version 140

in vec2 uv;
uniform sampler2D texture;
uniform vec3 textColor;

out vec4 color;

void main()
{
   color = vec4(1.0f, 1.0f, 1.0f, texture2D(texture, uv).r) * vec4(textColor, 1.0f);
}
