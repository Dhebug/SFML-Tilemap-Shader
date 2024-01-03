//
// Copyright (c) 2012 by Mickaël Pointier. 
// This work is made available under the terms of the Creative Commons Attribution-ShareAlike 3.0 Unported license,
// http://creativecommons.org/licenses/by-sa/3.0/.
//

#include <SFML/Graphics.hpp>

// This is the visual decription of the space station:
// 0 -> Dark Blue Starfield
// 1 -> Metal plates
// 2 -> Orange painted plates
char spaceStationAlpha[8][8] = 
{
  {0,0,0,0,0,0,0,0},
  {0,2,1,0,0,2,2,0},
  {0,0,1,1,1,2,2,0},
  {0,0,1,0,0,1,1,0},
  {0,0,1,0,0,1,1,0},
  {0,0,1,0,0,1,1,0},
  {0,2,1,1,1,1,1,0},
  {0,0,0,0,0,0,0,0}
};

int main()
{
  // Creates a 800x600 render window, with vsync enabled.
  sf::RenderWindow renderWindow(sf::VideoMode(800,600),"Tilemap example");
  renderWindow.setVerticalSyncEnabled(true);

  sf::Shader  tilemapShader;        // Loads and manages the vertex and fragment shader
  sf::Texture tileGraphicsTexture;  // GPU texture containing the 32x32 texture atlas image 

  // Load the shader
  if (! (tilemapShader.loadFromFile("tilemap.vert","tilemap.frag") && 
         tileGraphicsTexture.loadFromFile("texture_atlas.png")) )
  {
    // Either failed to load the shaders or the texture atlas
    return EXIT_FAILURE;
  }

  // This will allow us to acces the texture atlas image from the pixel shader.
  // Matches the 'uniform sampler2D tileGraphics;' line in the fragment shader.
  tilemapShader.setParameter("tileGraphics",tileGraphicsTexture);

  // We could have loaded another image containing the actual tilemap description,
  // but it seemed simpler to make it by code.
  //
  // The tilemap image is by default initialized with the black color, which in our
  // case means it is going to be using the starfield texture (index zero).
  //
  // The red component is used to store the index of the texture to display, which
  // limits to 256 different textures, but technically you can use the other channels
  // as well. Also the pixel format of the texture does not really matter. You can
  // change it so it matches your needs: Just make sure that the pixel shader is aware
  // of the change :)
  //
  // Since the texture is 256x256 pixel, I decided to put the space station graphics
  // in the center, which is at position 128x128. Since the station itself is 8x8 tiles
  // the station is created from position 124x124 to 132x132.
  //
  sf::Image tileMapImage;
  tileMapImage.create(256,256);
  for (int row=0;row<8;row++)
  {
    for (int column=0;column<8;column++)
    {
      sf::Color tileIndex(spaceStationAlpha[row][column],0,0,0);
      tileMapImage.setPixel(124+column,124+row,tileIndex);
    }
  }

  // The sf::Image is a CPU side image, it needs to be transfered to video memory
  // before it can be used by the graphic card.
  sf::Texture tileMapTexture;
  tileMapTexture.loadFromImage(tileMapImage);

  // Add the texture as a usable shader parameter
  tilemapShader.setParameter("tilemap",tileMapTexture);

  // This is where the magic happens:
  //
  // We tell the sf::Sprite engine it will be using the tilemap texture which is
  // 256 by 256 pixels, but then we will tell it to use texture coordinates that
  // would a 4096 by 4096 pixel texture use.
  //
  // The trick is that we want our pixel shader to draw the actual final texture
  // as if it had always be present in memory. This makes it possible to use the
  // tilemap very easy, no need to do complex tricks to clip it or rescale it:
  // Just pretend it has the size you wanted it to be in first place :)
  sf::Sprite tileMapSprite;
  tileMapSprite.setTexture(tileMapTexture);
  tileMapSprite.setTextureRect(sf::IntRect(0,0,256*16,256*16));
  tileMapSprite.setOrigin(128*16,128*16);


  float tilemapAngle=0.0f;
  float tilemapZoom =0.0f;

  // Start the game loop
  while (renderWindow.isOpen())
  {
    // Process events
    sf::Event event;
    while (renderWindow.pollEvent(event))
    {
      // Close window: exit
      if (event.type == sf::Event::Closed)
      {
        renderWindow.close();
      }
    }

    // Display the tilemap centered in the window
    tileMapSprite.setPosition(sf::Vector2f(renderWindow.getSize())/2.0f);

    // Update rotation
    tileMapSprite.setRotation(tilemapAngle);
    tilemapAngle+=0.1f;

    // Update zoom
    float scale=2.0f+1.0f*cosf(tilemapZoom);
    tileMapSprite.setScale(scale,scale);
    tilemapZoom+=0.01f;

    // Draw the tilemap
    // Nothing special there, just don't forget  to set the shader.
    sf::RenderStates tilemapStates;
    tilemapStates.shader=&tilemapShader;
    renderWindow.draw(tileMapSprite,tilemapStates);

    // Finally, display the rendered frame on screen
    renderWindow.display();
  }
  return EXIT_SUCCESS;
}
