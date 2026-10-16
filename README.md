# Particle Life
A particle life simulation built in C using raylib. 

Particle interaction is defined by a square attraction matrix.
- Negative values -> Repel
- Positive values -> Attract

Different particle types have set attraction factors defined by the matrix.
- The row defines the reacting on particle. (The particle feeling the force)
- The colomn defines that particle acting on said particle. (Applying a force) 

For example: Blue = b, Red = r
-         b    r
      b {  .5,  1}
      r {-.5,  .5}

Blue and red both are decently attracted to themselves, 
but blue is fully attracted by red, while red is decently repelled by blue.

These simple rules allow for complex emerging behavior when scaled, 
giving the apperance of simple life, hence the "life" bit.

Project built using MacOS raylib template here:
-     https://github.com/Ippokratis/raylib-mac-vscode

If Windows or Linux, figure it out.
