/* ----------------------------------------------------------------------------
	RayTraceRed.frag


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
uniform sampler2D backfaceBuffer;
uniform sampler2D frontfaceBuffer;
uniform sampler1D colorMapTexture;
uniform sampler1D windowLevelTexture;
uniform vec3 camera;
uniform int numIts;
uniform vec3 volExtentMin;
uniform vec3 volExtentMax;

varying vec4 pos;

void main()
{
	int i;
	float maxDist;
	float stepSize;
	
	/* get maximum possible ray length, and step size */
	maxDist = distance(volExtentMin,volExtentMax);
	stepSize = 1.5/float(numIts);

	/* initialize accumulated color and opacity */
	vec4 dst = vec4(0.0, 0.0, 0.0, 0.0);

	/* calculate back and front position on the culled cubes */
	vec4 back_position = texture2D(backfaceBuffer,gl_TexCoord[0].st);
	vec4 front_position = texture2D(frontfaceBuffer,gl_TexCoord[0].st);
	/* starting position for the ray is the front face position */
	vec3 start = front_position.xyz;
	vec3 dir = vec3(0.0, 0.0, 0.0);
	/* determine the direction and length of the ray */
	dir = back_position.xyz - front_position.xyz;
	float len = length(dir.xyz);
	vec3 norm_dir = normalize(dir);
  
	float delta = stepSize;
	vec3 delta_dir = norm_dir * delta;
	float delta_dir_len = length(delta_dir);
	vec3 vec = start.xyz;
	vec4 col_acc = vec4(0.0, 0.0, 0.0, 0.0);
	float alpha_acc = 0.0;
	float length_acc = 0.0;
	vec4 color_sample;
	float alpha_sample;
	vec3 src = vec3(0.0, 0.0, 0.0);
  
	/* loop for ray traversal */
	for (i=0; i<numIts; i++) { /* some large number */
		/* get the color from the 3D texture */
		color_sample = texture3D(mainTexture,vec);
		/* do a lookup in the color map */
		color_sample = texture1D(colorMapTexture,color_sample.r);

		alpha_sample = color_sample.a * stepSize;
		col_acc   += (1.0 - alpha_acc) * color_sample * alpha_sample * 3.0;
		alpha_acc += alpha_sample;
		
		vec += delta_dir;
		length_acc += delta_dir_len;
		if(length_acc >= len || alpha_acc > 1.0) break; // terminate if opacity > 1 or the ray is outside the volume
	}
	
	gl_FragColor = col_acc;
}
