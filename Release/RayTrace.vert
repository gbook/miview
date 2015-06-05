varying vec4 pos;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	pos = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = ftransform();
}
