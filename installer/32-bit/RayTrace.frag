/* this fragment program was adapted from "Real-time Volume Graphics" (page 168) */

uniform sampler3D mainTexture;
uniform sampler1D transferFunction;
uniform vec3 camera;
uniform float stepSize;
uniform vec3 volExtentMin;
uniform vec3 volExtentMax;

void main()
{
	vec4 value;
	float scalar;
	
	/* initialize accumulated color and opacity */
	vec4 dst = vec4(0.0, 0.0, 0.0, 0.0);
	
	/* determine volume entry position */
	vec3 position = gl_TexCoord[0].xyz;
	
	/* compute ray direction */
	vec3 direction = gl_TexCoord[0].xyz - camera;
	direction = normalize(direction);
	
	/* loop for ray traversal */
	for (int i=0; i< 50; i++) { /* some large number */
		/* data access to scalar value in 3D volume texture */
		value = texture3D(mainTexture,position);
		scalar = value.a;
		/* apply transfer function */
		vec4 src = texture1D(transferFunction, scalar);
		/* front to back compositing */
		dst = (1.0-dst.a) * src + dst;
		/* advance ray position along ray direction */
		position = position + direction * stepSize;
		/* ray termination, test if outside volume ... */
		vec3 temp1 = sign(position = volExtentMin);
		vec3 temp2 = sign(volExtentMax - position);
		float inside = dot(temp1,temp2);
		/* ... and exit the loop if it is */
		if (inside < 3.0) {
			break;
		}
	}
	
	//gl_FragColor = dst;
	gl_FragColor = vec4(0.0,1.0,0.0,1.0);
}
