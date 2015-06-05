/* ----------------------------------------------------------------------------
	RayTrace.frag


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
//uniform sampler1D windowLevelTexture;
//uniform vec3 camera;
uniform int numIts;
uniform int useSSD;
//uniform vec3 volExtentMin;
//uniform vec3 volExtentMax;

varying vec4 pos;

void main()
{
	int i;
	float stepSize;
	
	/* get maximum possible ray length, and step size */
	stepSize = 1.5/float(numIts);

	vec4 finalColor = vec4(0.0,0.0,0.0,1.0);
	float value;
	
	/* initialize accumulated color and opacity */
	//vec4 dst = vec4(0.0, 0.0, 0.0, 0.0);

	/* calculate back and front position on the culled cubes */
	vec4 backPosition = texture2D(backfaceBuffer,gl_TexCoord[0].st);
	vec4 frontPosition = texture2D(frontfaceBuffer,gl_TexCoord[0].st);
	/* starting position for the ray is the front face position */
	vec3 start = frontPosition.xyz;
	vec3 dir = vec3(0.0, 0.0, 0.0);
	/* determine the direction and length of the ray */
	dir = backPosition.xyz - frontPosition.xyz;
	float len = length(dir.xyz);
	vec3 normDir = normalize(dir);
  
	float delta = stepSize;
	vec3 deltaDir = normDir * delta;
	float deltaDirLen = length(deltaDir);
	vec3 vec = start.xyz;
	vec4 colAcc = vec4(0.0, 0.0, 0.0, 0.0);
	float alphaAcc = 0.0;
	float lengthAcc = 0.0;
	vec4 colorSample;
	float alphaSample;
	//vec3 src = vec3(0.0, 0.0, 0.0);
  
	/* loop for ray traversal */
	for (i=0; i<numIts; i++) { /* some large number */
		if ((len > 0.025) && (len < 0.075)) {
			colAcc = vec4(1.0,1.0,1.0,1.0); break;
		}
		if ((len > 0.0) && (len < 0.01)) {
			colAcc = vec4(0.5,0.5,0.5,1.0); break;
		}

		/* get the color from the 3D texture */
		colorSample = texture3D(mainTexture,vec);
		
		//if (useSSD == 1) {
		//	/* terminate if the color is > 0.5 */
		//	if (colorSample.r > 0.2) {
		//		value = (1.0 - (float(i)/float(numIts)))*colorSample.r;
		//		finalColor = vec4(value,value,value,1.0);
		//		break;
		//	}
		//}
		
		/* do a lookup in the color map */
		colorSample = texture1D(colorMapTexture,colorSample.r);

		alphaSample = colorSample.a * stepSize;
		colAcc   += (1.0 - alphaAcc) * colorSample * alphaSample * 3.0;
		alphaAcc += alphaSample;
		
		vec += deltaDir;
		lengthAcc += deltaDirLen;
		
		/* terminate if opacity > 1 or the ray is outside the volume */
		if(lengthAcc >= len || alphaAcc > 1.0) break;
	}
	
	if (useSSD == 1) {
		gl_FragColor = finalColor;
	}
	else {
		gl_FragColor = colAcc;
	}
}
