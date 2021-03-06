//
// Fragment Shader f�r 7x7 Gauss Tiefpassfilter
// Angepasst f�r Core Profile
// ---------------------------------
//
// @author: Prof. Dr. Alfred Nischwitz
// @lecturer: Prof. Dr. Alfred Nischwitz
//
// (c)2017 Hochschule M�nchen, HM
//
// ---------------------------------
//uniform vec2 offsets[49] = vec2[](	vec2(-3,  3), vec2(-2,  3), vec2(-1,  3), vec2(0,  3), vec2(1,  3), vec2(2,  3), vec2(3,  3), 
//										vec2(-3,  2), vec2(-2,  2), vec2(-1,  2), vec2(0,  2), vec2(1,  2), vec2(2,  2), vec2(3,  2),
//										vec2(-3,  1), vec2(-2,  1), vec2(-1,  1), vec2(0,  1), vec2(1,  1), vec2(2,  1), vec2(3,  1),
//										vec2(-3,  0), vec2(-2,  0), vec2(-1,  0), vec2(0,  0), vec2(1,  0), vec2(2,  0), vec2(3,  0),
//										vec2(-3, -1), vec2(-2, -1), vec2(-1, -1), vec2(0, -1), vec2(1, -1), vec2(2, -1), vec2(3, -1),
//										vec2(-3, -2), vec2(-2, -2), vec2(-1, -2), vec2(0, -2), vec2(1, -2), vec2(2, -2), vec2(3, -2),
//										vec2(-3, -3), vec2(-2, -3), vec2(-1, -3), vec2(0, -3), vec2(1, -3), vec2(2, -3), vec2(3, -3)	);


#version 330

smooth in vec2 texCoords;				// pixelbezogene Texturkoordinate
out vec4 fragColor;					// Ausgabewert mit 4 Komponenten zwischen 0.0 und 1.0
uniform sampler2DRect textureMap;		// Sampler f�r die Texture Map
uniform vec4 param1;					// Param1 X,Y,Z,W in GUI

uniform vec2 offsets[7] = vec2[](		vec2(-3,  0), vec2(-2,  0), vec2(-1,  0), vec2(0,  0), vec2(1,  0), vec2(2,  0), vec2(3,  0));

void main()

// um die 95 FPS

{

	/*vec4 texel;
	float pi = 3.14159265359;
	float h;
	float counter = 0.0 ;
	float p1 = param1[0] / 100;


	if (p1 != 0) {
		for (int i = 0; i < 49; i++) {
			float x = offsets[i][0];
			float y = offsets[i][1];

			h  = (1 / (2 * pi * p1 * p1)) * exp(-(x * x + y * y) / (2 * p1 * p1));
		
			texel +=  texture(textureMap, texCoords + offsets[i]) * h;
			counter += h;
		}
			
		fragColor = texel / counter;

	} else 
		{
		fragColor = texture(textureMap, texCoords);
		}
		*/

	vec4 texel;
	float pi = 3.14159265359;
	float h;
	float counter = 0.0 ;
	float p1 = param1[0] / 100;

	if (p1 != 0) {
	
		for (int i = 0; i < 7; i++) {
				float x = offsets[i][0];

				h  = (1 / (p1 * sqrt(2 * pi))) * exp(-(x * x) / (2 * p1 * p1));
		
				texel +=  texture(textureMap, texCoords + offsets[i]) * h;
				counter += h;
			}
			
			fragColor = texel / counter;


	} else {
		fragColor = texture(textureMap, texCoords);
	}

}