//----------------------------------------------------------------------------------------
/**
 * \file       shaderData.h
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#pragma once
/// ===========  SHADERS ===========
 const std::string colorVertexShaderSrc(
	"#version 140\n"
	"uniform mat4 PVMmatrix;\n"
	"uniform float cm;\n"
	"in vec3 position;\n"
	"in vec3 color;\n"	
	"smooth out vec4 theColor;\n"
	"void main() {\n"
	"  gl_Position = PVMmatrix * vec4(position, 1.0);\n"
	"  theColor = vec4(cm * color, 1.0);\n"
	"}\n"
);

const std::string colorFragmentShaderSrc(
	"#version 140\n"
	"smooth in vec4 theColor;\n"
	"out vec4 outputColor;\n"
	"void main() {\n"
	"  outputColor = theColor;\n"
	"}\n"
);

const std::string boxVertexShaderSrc(
	"#version 140\n"
	"uniform mat4 PVMmatrix;\n"
	"uniform float cm;\n"
	"in vec3 position;\n"
	"in vec3 color;\n"
	"in vec2 texCoord;\n"

	"smooth out vec4 theColor;\n"
	"out vec2 oTexCoord;\n"

	"void main() {\n"
	"	gl_Position = PVMmatrix * vec4(position, 1.0);\n"
	"	theColor = vec4(cm * color, 1.0);\n"
	"	oTexCoord = texCoord;\n"
	"}\n"
);

const std::string boxFragmentShaderSrc(
	"#version 140\n"
	"smooth in vec4 theColor;\n"
	"in vec2 oTexCoord;\n"
	"uniform sampler2D ourTexture;\n"

	"out vec4 outputColor;\n"

	"void main() {\n"
	"	outputColor = texture(ourTexture, oTexCoord);\n"
	"}\n"
);