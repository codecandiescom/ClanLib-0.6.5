         Name: Mouse
       Status: Windows(Y), Linux(Y)
        Level: Beginner
      Modules: app, core, display
Last modified: 28. februar 2002
     Progress: Complete
   Maintainer: Core developers
      Summary: Mouse, custom mouse cursor, mouse buttons & wheel

The mouse example will show you how to use a custom (animated) mousecursor in
your ClanLib application. It will also show how to react to buttonpresses &
wheel movement.

You will see a window with a blue background. If you move your mousecursor
into this window, the mousecursor will change shape into a hand. This is an
animated hand, moving it's index finger to point at anything you're pointing
the mouse cursor too. If you click anywhere in the window, a message on the
console screen will appear stating what button you pressed and where.

Known issues:
- Segfaults at exit under Linux.  (Harry, 25. feb, 2002)
