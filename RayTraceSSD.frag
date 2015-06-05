/* ----------------------------------------------------------------------------
	RayTraceSSD.frag


	MIView - Medical Image Viewer
    Copyright (C) 2009  Gregory Book

    MIView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	--------------------------------------------------------------------------- */

/* this fragment program was adapted from "Real-time Volume Graphics" (page 168) */

uniform sampler3D mainTexture;
uniform sampler1D colorMapTexture;
uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform vec3 position;
varying vec4 pos;

vec3 shading(in vec3 N, in vec3 V, in vec3 L)
{
	/* material properties */
	vec3 Ka = vec3(0.1, 0.1, 0.1); /* ambient */
	vec3 Kd = vec3(0.6, 0.6, 0.6); /* diffuse */
	vec3 Ks = vec3(0.2, 0.2, 0.2); /* specular */
	float shiney = 100.0;
	
	/* light properties */
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 ambientLight = vec3(0.3, 0.3, 0.3);
	
	/* calculate halfway vector */
	vec3 H = normalize(L + V);
	
	/* compute ambient term */
	vec3 ambient = Ka * ambientLight;
	
	/* compute diffuse term */
	float diffuseLight = max(dot(L,N), 0.0);
	vec3 diffuse = Kd * lightColor * diffuseLight;
	
	/* compute specular term */
	float specularLight = pow(max(dot(H,N),0.0), shiney);
	if (diffuseLight <= 0.0) specularLight = 0.0;
	vec3 specular = Ks * lightColor * specularLight;
	
	vec3 retVal = ambient + diffuse + specular;
	return retVal;
	//return ambient + diffuse + specular;
}

void main()
{
	float delta = 0.01;
	float threshold = 0.1;
	
	vec3 uvw = gl_TexCoord[0].xyz;
	
	/* one texture sample for the scalar value */
	float sample = texture3D(mainTexture,uvw).r;

	/* emission and absorption from the transfer function */
	vec4 result = texture1D(colorMapTexture,sample);
	
	if (result.a > threshold) {
		vec3 sample1, sample2;
		
		/* six texture samples for the gradient */
		sample1.r = texture3D(mainTexture,uvw-vec3(delta, 0.0, 0.0)).r;
		sample2.r = texture3D(mainTexture,uvw+vec3(delta, 0.0, 0.0)).r;

		sample1.g = texture3D(mainTexture,uvw-vec3(0.0, delta, 0.0)).r;
		sample2.g = texture3D(mainTexture,uvw+vec3(0.0, delta, 0.0)).r;
		
		sample1.b = texture3D(mainTexture,uvw-vec3(0.0, 0.0, delta)).r;
		sample2.b = texture3D(mainTexture,uvw+vec3(0.0, 0.0, delta)).r;
		
		/* central difference and normalization */
		vec3 N = normalize(sample2-sample1);
		
		/* calculate light and viewing directions */
		vec3 L = normalize(lightPosition - position);
		vec3 V = normalize(eyePosition - position);
		
		/* add local illumination */
		result.rgb += shading(N,V,L);
	}
	
	gl_FragColor = result;
}
