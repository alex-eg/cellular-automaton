uniform vec2 WindowSize;

void main(void)
{
   vec4 a = gl_Vertex;
   vec4 b;
   
   a.x = -a.x + WindowSize.x / 2;
   a.y = -a.y + WindowSize.y / 2;

   gl_Position = gl_ModelViewProjectionMatrix * a;
}
