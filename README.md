Graue Utils
===========

Small miscellaneous command-line tools I have found useful
to keep around.

Some of these are many zillions of years old. They were not
under version control before 2012, but got copied from
machine to machine.

But now... it's the future and everything is in git!

The one I think is coolest is:

sponge
------

Sponge lets you use a pipe to seemingly modify a file "in
place":

    sort <foo | sponge foo

It works by creating a temporary file that sucks down all
the input. Finally, this gets moved to the given filename.

I'll admit I haven't actually used this program in years,
but it works and seemed clever as hell when I first made it.
