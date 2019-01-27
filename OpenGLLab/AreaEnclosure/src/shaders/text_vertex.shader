
in vec4 position;
out vec2 textCoord;

void main()
{
    gl_Position = vec4(position.xy, 0, 1);
    textCoord = position.zw;
}