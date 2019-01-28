         Name: ScreenShot
       Status: Windows(Y), Linux(Y)
        Level: Intermediate
      Modules: app, core, display, png
Last modified: 16. october 2001
     Progress: Complete
   Maintainer: Core developers
      Summary: screen shot, save png

This example shows how to get the screen as a CL_Surface and how to save the
CL_Surface as a PNG using libPNG. The PNG saving code is a hack; tt the time
of writing ClanLib itself is NOT able to save images. It is planned in a future
version of ClanLib, though.

You will see a screen, can press space and the screen will be saved to a png file.
You can look at this png file with your favourite picture viewer, and see if it
is the same as shown on the screen.
