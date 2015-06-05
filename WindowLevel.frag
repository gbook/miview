/* ----------------------------------------------------------------------------
	WindowLevel.frag


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

uniform float minVal;
uniform float multiplier;
uniform bool useColorMap;
uniform bool useLUT;
uniform sampler2D mainTexture;
uniform sampler1D lutTexture;
uniform sampler1D colorMapTexture;

void main()
{
	vec4 finalCol;
	float tmpColor;
	
	vec4 color = vec4(texture2D(mainTexture, gl_TexCoord[0].st)) * multiplier;
	color.rgb -= minVal;

	if (useLUT == true) {
		if (useColorMap == true) {
			/* apply the window level LUT first ... */
			tmpColor = texture1D(lutTexture, color.r).r;
			/* ... then apply the LUT to each color channel */
			finalCol = vec4(texture1D(colorMapTexture, tmpColor));
		}
		else {
			/* don't apply color map, so just use the color from above */
			finalCol.r = texture1D(lutTexture, color.r).r;
			finalCol.g = texture1D(lutTexture, color.g).g;
			finalCol.b = texture1D(lutTexture, color.b).b;
			finalCol.a = color.a;
		}
	}
	else {
		finalCol = color;
	}
	
	//gl_FragColor = vec4(finalCol.a,0.0,0.0,1.0);
	gl_FragColor = finalCol;
}
