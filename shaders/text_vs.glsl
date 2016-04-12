attribute vec4 position;
in vec2 texcoord;
uniform mat4 MVPmat4;

out vec2 uv;

void main(void)
{
    gl_Position = MVPmat4 * vec4(position.xy, 0, 1);
    uv = position.zw;

}
