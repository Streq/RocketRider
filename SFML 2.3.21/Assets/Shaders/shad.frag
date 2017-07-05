uniform sampler2D texture;
uniform float blur_radius;

void main()
{
    vec2 offx = vec2(blur_radius, 0.0);
    vec2 offy = vec2(0.0, blur_radius);

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	pixel += vec4(0.f,1.f,0.f,0.f);

    gl_FragColor =  gl_Color * pixel;
}
