//
// Copyright (c) 2012 by Mickaël Pointier. 
// This work is made available under the terms of the Creative Commons Attribution-ShareAlike 3.0 Unported license,
// http://creativecommons.org/licenses/by-sa/3.0/.
//
#version 130

void main() 
{
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_FrontColor  = gl_Color;
}

