#version 140

smooth in vec4 theColor;
in vec2 oTexCoord;
uniform sampler2D ourTexture;

out vec4 outputColor;

void main() {
	outputColor = texture(ourTexture, oTexCoord);
}
/*
	smooth in vec4 theColor;
	out vec4 outputColor;
	void main() {
	  outputColor = theColor;
	}
	*/