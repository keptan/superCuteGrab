# superCuteGrab
Booru image tag-fetching and ranking with Microsoft TRUESKIL and IPTC metadata
>minimum requirements for the binary to run

must be in the same directory as icon.png and window.glade

>how to tag images with metadata

./cutegrab danTest <location of images>

be very careful and please backup your images; please note that it searches for images RECURSIVELY meaning if you run it on a top level directory it might tag thousands of images you didnt want too
it might also mess with the last modified date
id run on a backup of images and keep your originals seperate to be extra safe

>how to search for a tag in cli

./cutegrab searchTest <location> <tag>
this will find images with that tag for you

>how to do tag ranking with trueskill

./cutegrab gtkTest <location>
first filter the images down with a MD5: search because all tagged images contain a MD5: tag
this way you dont rank untagged images; desu i dont even remember what that does if you accideentally do that

select an image, and then use the left and right arrows to choose which one is cuter
results are in the database folder and filebase folder
also it sorts all your searches by cutescore
you can browse images in the gui too if you want
thumbnails wont appear unless you scroll

sorry if you have any problems with my crappy software; I didnt really expect anyone else to use it
please tell me any questions or problems you have
ill rewrite it soon!

the up arrow indicates that the two images are equally cute
and if the aspect ratio of the image is all messed up use the down arrow to fix that

use freesoftware like libreoffice or gnuplot to look at your cute tag results after
