R"(
#version 330 core

uniform sampler2D tex;

uniform vec2 tex_size;
uniform vec2 uv;
uniform vec2 crop_size;

in vec2 TexCoord;

layout(location = 0) out vec4 color;

void main()
{
    color = texture(tex, TexCoord);
}
)"