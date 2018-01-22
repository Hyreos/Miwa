R"(
#version 330 core

layout(location=0) in vec3 inPosition;
layout(location=1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform vec2 tex_size;
uniform vec2 uv;
uniform vec2 crop_size;

out vec2 TexCoord;

void main()
{
    vec2 real_uv = vec2(uv.x/tex_size.x, uv.y/tex_size.y);
    vec2 real_size = vec2(crop_size.x/tex_size.x, crop_size.y/tex_size.y);
    TexCoord = vec2(real_uv.x+(real_size.x*texCoord.x), uv.y+(real_size.y*texCoord.y));
    gl_Position = projection * view * model * vec4(uv.x+(crop_size.x*texCoord.x), uv.y+(crop_size.y*texCoord.y), inPosition.z, 1.0);
}
)"