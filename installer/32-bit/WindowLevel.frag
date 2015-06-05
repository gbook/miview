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
