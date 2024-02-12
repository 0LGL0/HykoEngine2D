#version 450 core
#extension GL_ARB_gpu_shader_int64: require
#extension GL_ARB_bindless_texture: require

#define OPACITY_THRESHOLD 0.000001

out vec4 outColor;

in vec3 color;
in vec2 textureCoord;
flat in float textureIDIndx;

//layout(binding = 0) uniform sampler2D tex[32];

layout(bindless_sampler) uniform;

layout(std430, binding = 1) buffer textureHadles {
	sampler2D handles[];
};

void main(){
	vec4 textureColor = vec4(1.0);

	int tIndx = int(textureIDIndx);

	if(textureIDIndx != -1.0) {
//		switch(int(textureIDIndx)){
//			case 0:  textureColor = texture(tex[0 ], textureCoord); break;
//			case 1:  textureColor = texture(tex[1 ], textureCoord); break;
//			case 2:  textureColor = texture(tex[2 ], textureCoord); break;
//			case 3:  textureColor = texture(tex[3 ], textureCoord); break;
//			case 4:  textureColor = texture(tex[4 ], textureCoord); break;
//			case 5:  textureColor = texture(tex[5 ], textureCoord); break;
//			case 6:  textureColor = texture(tex[6 ], textureCoord); break;
//			case 7:  textureColor = texture(tex[7 ], textureCoord); break;
//			case 8:  textureColor = texture(tex[8 ], textureCoord); break;
//			case 9:  textureColor = texture(tex[9 ], textureCoord); break;
//			case 10: textureColor = texture(tex[10], textureCoord); break;
//			case 11: textureColor = texture(tex[11], textureCoord); break;
//			case 12: textureColor = texture(tex[12], textureCoord); break;
//			case 13: textureColor = texture(tex[13], textureCoord); break;
//			case 14: textureColor = texture(tex[14], textureCoord); break;
//			case 15: textureColor = texture(tex[15], textureCoord); break;
//			case 16: textureColor = texture(tex[16], textureCoord); break;
//			case 17: textureColor = texture(tex[17], textureCoord); break;
//			case 18: textureColor = texture(tex[18], textureCoord); break;
//			case 19: textureColor = texture(tex[19], textureCoord); break;
//			case 20: textureColor = texture(tex[20], textureCoord); break;
//			case 21: textureColor = texture(tex[21], textureCoord); break;
//			case 22: textureColor = texture(tex[22], textureCoord); break;
//			case 23: textureColor = texture(tex[23], textureCoord); break;
//			case 24: textureColor = texture(tex[24], textureCoord); break;
//			case 25: textureColor = texture(tex[25], textureCoord); break;
//			case 26: textureColor = texture(tex[26], textureCoord); break;
//			case 27: textureColor = texture(tex[27], textureCoord); break;
//			case 28: textureColor = texture(tex[28], textureCoord); break;
//			case 29: textureColor = texture(tex[29], textureCoord); break;
//			case 30: textureColor = texture(tex[30], textureCoord); break;
//			case 31: textureColor = texture(tex[31], textureCoord); break;
//		}

		textureColor = texture(handles[tIndx], textureCoord);

		// alpha-test
		if(textureColor.a < OPACITY_THRESHOLD)
			discard;
	}
	
	outColor = textureColor * vec4(color, 1.0);
}
