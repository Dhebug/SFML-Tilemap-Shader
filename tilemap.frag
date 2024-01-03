//
// Copyright (c) 2012 by Mickaël Pointier. 
// This work is made available under the terms of the Creative Commons Attribution-ShareAlike 3.0 Unported license,
// http://creativecommons.org/licenses/by-sa/3.0/.
//
#version 130

uniform sampler2D tilemap;
uniform sampler2D tileGraphics;

void main()
{
  // Since the tilemap is being rendered with texture coordinates that matches
  // the final tilemap texture resolution, we need to divide the texture coordinates
  // by the size of a tile to get the coordinates of the tile we want to render.
  //
  vec2 tilePos=gl_TexCoord[0].xy/16;

  // Fetch the color of the texel from the tile coordinates.
  // The red component contains the index of the tile, since the value is stored in
  // homogeneous coordinates it needs to be multiplied by 256.
  //
  float index = floor(texture2D(tilemap,tilePos).r*256);

  // From the index, we can compute the location of the tile texture we want in the
  // texture atlas. We have four textures in the atlas: 
  //
  //   0    0.5  1
  //     +---+---+
  //     | 0 | 1 |
  // 0.5 +---+---+
  //     | 2 | 3 |
  //   1 +---+---+
  // 
  // Two textures are on the first row, and the two others on the second row.
  // - index modulo 2 returns the column (0 or 1)
  // - index divided by 2 returns the row (0 or 1)
  // Multiply by 0.5 to get the values in the range { 0 , 0.5 }
  //
  vec2 baseTilePos=0.5*floor(vec2(mod(index,2),index/2)); 

  // Compute the internal texel coordinates of the 16x16 texture in the 32x32 atlas
  // The final value we want is in the range  { 0 , 0.5 } so that when adding
  // the base tile position the value we get stays in the 0 to 1.0 range.
  //
  // - Take the input texture coordinate
  // - multiply by 16 (size of the texture in the atlas)
  // - modulo 1 to map to the { 0 , 1.0 } range
  // Multiply by 0.5 to get the values in the range { 0 , 0.5 }
  //
  vec2 internalPos=0.5*mod(gl_TexCoord[0].xy*16,1);

  // Finally fetch the texel but adding the two positions
  //
  gl_FragColor=texture2D(tileGraphics,baseTilePos+internalPos);
}

