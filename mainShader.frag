// uniform sampler2D texture;
uniform vec2 middlePoint;
void main()
{
      float brightness = 3.5;
      float dt = 1.0/brightness - distance(gl_FragCoord.xy, middlePoint)/(120.0*brightness);
      gl_FragColor = vec4(1.0,1.0,1.0,dt);
}
