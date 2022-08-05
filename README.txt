What is this?

Cursor Tracker is a program that can be used for storing your mouse location data (related to the time and whether left click was on or not)
in txt files it creates in it's own directory ,these files make use of a very simple storing protocol and and can be easily modifiable.
It then can use these txt files to create a window and visualize the data as a 2d ,appropriately sized canvas ,in a video like manner.
The window is created using the only non standart dependency ,SFML ,a library used for 2d-3d object creation and visualization,
it is working in a very similar manner to libraries like pygame or GLUT extention for openGL.

How does it work?

Two threads that are used for mouse input and visual output are created ,they are attached and detached at appropriate times,
second thread creates and modifies bitmap files in real time rendering it rather slow.
Threaded functions are also using global variables as to pass data to and fro main thread.More about this can be learned 
by using 'info' command in the program or looking at tools.cpp.

How can be used?

The program can be used to monitor mouse movements during a certain interval of time in an easy way ,albeit slow ,jump time command or simply 
moving the green time stamp rectangle you will see on the lower end of screen makes the visualisation process easy and renders this program 
viable for a full confortable use.

Is this program efficient?

Absolutely not ,but it isn't a far call either ,instead of creating an image we sould have created an mp4 file,
since it has a more complicated extention it is way harder to design it's buffer ,
after which we could have used a simple media player library for the visualization of the files.

Who did this?

It's a me ,3urakKardas on github ,for any idea and request please contact me.

License?

The program and the code can be used ,modified at full extent ,since updating an image and uploading it during the runtime is a slow
and rather buggy process I do NOT recommand this approach nor do I take responsability.