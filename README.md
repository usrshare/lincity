
                     **  Welcome to lincity  **

This is the README for lincity 1.14 (development version)

# WHAT IS LINCITY

Lincity is a cross-platform city/country simulation game. It is based on the SDL
library and is supposed to run on any systems where it is available.

You are required to build and maintain a city. You must feed, house, 
provide jobs and goods for your residents. You can build a sustainable
economy with the help of renewable energy and recycling, or you can go for
broke and build rockets to escape from a pollution ridden and resource
starved planet, it's up to you. Due to the finite resources available in any
one place, this is not a game that you can leave for long periods of time.

This version of Lincity is being hosted on GitHub. Since the last official
version of this game has been released in 2004, this is to be considered an
independent fork. Lincity's concept and code have since also been used in
[Lincity-NG](https://github.com/lincity-ng/lincity-ng), a pseudo-3D isometric
game with lots of changes and enhancements.

# COMPILATION AND INSTALLATION

In order to compile Lincity, the following libraries are required (as well as
the corresponding development files):

* SDL 1.2 or newer, SDL\_ttf 2.0 or newer.
* libpng,
* libz.

To install lincity, extract the supplied archive or clone the Git repository
into a folder, then open the terminal and type the following commands:

    cmake .
    make

This will compile Lincity, allowing it to be run out of the source directory.
But if you want to install, you can do this:

    sudo make install

# RUNNING LINCITY

Usually you will invoke the game with the `lincity` command.

    lincity

# UPGRADING FROM VERSION 1.13

Lincity 1.14 is so far perfectly compatible with Lincity 1.13, in the sense 
that 1.14 can read files generated in 1.13.  

# THE LINCITY HOME DIRECTORY

If you get a message such as "Error. Can't find LINCITY\_HOME", this
means that lincity can't find its home directory.  To find its 
home directory, lincity searches for the file "colour.pal" in 
the following three directories (in order):

	The directory pointed to by the $LINCITY_HOME environment variable
	The current working directory
	The default directory


# LINCITY CONFIGURATION FILE

Lincity now has a configuration file for saving defaults.  You can 
find it in the following location:
          
UNIX:       $HOME/.lincityrc
WINDOWS:    %LINCITY\_HOME%\lincity.ini

To reset to the "factory default" configuration, simply delete the 
file.  Lincity will regenerate the file if it is missing.


# LINCITY ARTWORK

Starting with version 1.13, you can (more easily) customize the lincity 
buildings and animations.  To modify the artwork simply edit the 
icon image file, located at opening/icons.png, with your favorite 
image editor.  

The icons.png file has a 256-color indexed format. The palette contained
within icons.png is used as the in-game palette. Some color indexes are
predefined, though, so it's better not to make radical modifications of
the PNG file's palette.

# INTERNATIONALIZATION UNDER LINUX

Lincity supports a modest amount of internationalization.  Only languages
that can be represented using a iso8859-1 font are currently supported.
I have created a lincity-compatible iso8859-2 font also, but have not used 
it yet because of no demand (send me an email if you would like to try).

If you wish to make a translation for your language, you will need to 
download and install gettext 0.11.2 or higher.

The first task is to get the sample Italian translation to display 
correctly.  Download a fresh lincity (1.12pre53 or higher) from
sourceforge.  Do a configure, make, and make install.

Next, download the Italian package.  Install as described in README.it.
Run as follows:

   export LANG=it\_IT          ## (on bash and similar)
   setenv LANG it\_IT          ## (on csh and similar)
   xlincity -D

Hopefully you will see something like this:

   Setting entire locale to it\_IT
   Setting messages locale to it\_IT
   Query locale is it\_IT
   GUESS 1 -- intl\_suffix is it\_IT
   Trying Message Path /home/gsharp/lincity/share/lincity/messages/it\_IT/
   Trying Help Path /home/gsharp/lincity/share/lincity/help/it\_IT/
   GUESS 2 -- intl\_suffix is it
   Trying Message Path /home/gsharp/lincity/share/lincity/messages/it/
   Set Message Path /home/gsharp/lincity/share/lincity/messages/it/
   Trying Help Path /home/gsharp/lincity/share/lincity/help/it/
   Set Help Path /home/gsharp/lincity/share/lincity/help/it/
   Bound textdomain directory is /home/gsharp/lincity/share/locale
   Textdomain is lincity
   DefaultVisual id=35 bp-rgb=8 map-entries=256


If you see this, then everything worked perfectly.  You should see perfect
Italian.  In particular, note the following:

  (1)  The opening screen (help page) is in Italian
  (2)  The month January will be written "Gen" instead of "Jan"
  (3)  Click on something that hasn't been invented yet, like a 
       power station or windmill.  The message for "Not enough tech" 
       will be in Italian.

It is possible that you see English for (1) and (3).  If so, you 
are probably running from the directory you built from.  Please cd 
out of that directory, and try again.

If you are less lucky, maybe only (1) and (3) worked correctly and 
but not (2).  If so, you probably saw something like this:

   Setting entire locale to (null)
   Setting messages locale to (null)
   Query locale is C
   GUESS 1 -- intl\_suffix is it\_IT
   Trying Message Path /usr/local/lincity/messages/it\_IT/
   Trying Help Path /usr/local/lincity/help/it\_IT/
   GUESS 2 -- intl\_suffix is it
   Trying Message Path /usr/local/lincity/messages/it/
   Set Message Path /usr/local/lincity/messages/it/
   Trying Help Path /usr/local/lincity/help/it/
   Settling for help Path /usr/local/lincity/help/
   Bound textdomain directory is /lincity/share/locale
   Textdomain is lincity
   DefaultVisual id=35 bp-rgb=8 map-entries=256

If this happens, it means that your system is missing some files which 
are require to "set the locale" of your system to "it\_IT".  Probably 
you are running Linux -- I don't think other Unix's have this peculiar 
requirement.

To check if this is the problem (on Linux), look for the directory 
/usr/lib/locale/it\_IT.  If this directory doesn't exist, you have this
problem.  To fix it, do the following:

   1)  Make a backup of the directory /usr/share/i18n/charmaps
   2)  Make a backup of the directory /usr/share/i18n/locale
   3)  cd /usr/share/i18n/charmaps
   4)  gunzip UTF-8.gz
   5)  gunzip ISO-8859-1.gz
   6)  cd /usr/share/i18n/locale
   7)  localedef -f ../charmaps/UTF-8 -i it\_IT it\_IT.UTF-8
   8)  localedef -f ../charmaps/ISO-8859-1 -i it\_IT it\_IT

Now check for the directory /usr/lib/locale/it\_IT.  It will be there.

OK, now you are ready to make translations for a new language.  
There are three parts to making a translation:
   making help files
   making message files
   making a .po file.

The messages and help files are just a matter of editing the ASCII 
files, and installing them.  If your language is "it", you should 
create two directories such as /usr/local/share/lincity/messages/it
and /usr/local/share/lincity/help/it.  Copy the English language 
help and message files to these directories, and translate them.

The .po file is a matter of translating these and getting them to compile 
correctly.  First, edit the file po/LINGUAS, and add your language 
(e.g. "it") to the list.  Next, create the .po file.  For the language
"it", the command looks like this like this:

   cd po
   msginit -l it

You should now see the file "it.po".  Edit this file.  Specifically,
you should edit what comes after "msgstr".  Don't erase any of the 
other stuff such as "msgid" or "#: cliglobs.c:54" or that stuff.
If you are using emacs, there is a "po-mode" to facilitate 
(or obstruct) this task.

When you are done editing this file, you need to compile this file.
Simply type "make" in the po directory to compile.  This will 
create the file "it.gmo".  Finally, you will need to install this file.
To install, you will copy this file to the following location:

  cp it.gmo /usr/local/share/locale/it/LC\_MESSAGES/lincity.mo

Now, you are ready to go.  Set the LANG environment variable and 
lincity should run using your translations.

Email to gregsharp@geocities.com for comments about this process.

# BUGS

Many.  See the file TODO.

If you wish to report a bug, you can send to directly to the current
maintainer or to the lincity mailing list.

# HISTORY

Ian Peters started writing lincity in June 1995 as an exercise in 
mouse control in SVGALIB. It quickly became a usable, then even fun, 
game to play. Over the next 18+ months it evolved into a challenging 
and stable game which, in a number of people's opinion, is worth uploading 
to the net.

In late 1997, lincity developers made grand plans for lincity version 2, 
an intergalactic space simulation with all the trimmings.  But from 
1998-1999, lincity underwent a period of hibernation, with only minor 
patches being applied.  

Development was restarted in late 1999 by Greg Sharp to provide
some moderate improvements, including networking support, GUI
improvements and additional gameplay developments.  Networking 
support was subsequently dropped in 2001.

In mid-2001, Greg was joined by Corey Keasling, who helped to bring
the game to sourceforge.net.  In late 2001, Corey became
co-maintainer.  Version 1.12 was released in December 2003.

Lincity's code used in this fork has also been maintained by Debian's lincity
maintainers to ensure it compiles and runs in modern environments.

In 2015, the Debian unstable version of Lincity has been used as the base for
this fork.

# COPYRIGHT

Lincity is copyrighted software.
Copyright (c) I J Peters 1995-1997.
Copyright (c) Greg Sharp 1997-2003.
Copyright (c) Corey Keasling 2000-2003.
Copyright (c) [Debian
maintainers](http://metadata.ftp-master.debian.org/changelogs//main/l/lincity/lincity_1.13.1-12_changelog)
1997-2015.
Copyright (c) usr\_share and github contributors 2015.

You may freely copy, distribute and modify lincity under the terms of the

                       GNU GENERAL PUBLIC LICENSE

Please read the file COPYING for the GPL.

THANK YOU
=========
This game could not have been possible without the patches, scripts, 
bug reports, suggestions, and other contributions from hundreds of 
developers and users around the world.  Please read the file 
'Acknowledgements' for details.

And a special thanks to *you* for playing and enjoying lincity!
