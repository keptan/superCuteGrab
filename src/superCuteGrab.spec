design:
	some big giant class heirarchy with inheretence and lots of std:: stuff
	

classes:
	Image
		holds file path, hash, filename
		|
		|_Metadata
			holds metadata from image, reads and writes tags
			|
			|_BooruInterface
				Does curl stuff to read tags from internet, keeps them seperately to metadata tags
	SkillHandle
		does calculations with tag, takes metadata pointers as input and calculates ELO data based on winner and loser
		|
		|_SkillDatum
			hold sinfo for a single tag, is manipulated by skillHandle
		- SkillBase
			holds info and parses the database of tags and their scores that the program has seen before

	Window
		holds GTkWidgets and such for gui


Notes:
	definitely generate some kind of page file to avoid searching through the filesystem constantly which is super slow, sorted by md5


Feature Ideas: (from most to least important)
								Done
ability to tag images			(!)
ability to search for tags		(!)
ability to rate images			(!)
ability to rate tags in team	(!)
manually tag images				(!)
gui shows two images			(!)
pagefile of images 
	holds a vector of all the imaegs in a directory(SLOW?)
	can fetch random images based on elo criteria 
	has method to write the metadatas of every image 
	at program end
	
seperate images from tags in db
keep stats overtime for tags
mark images with manual vs automatic tags
browse images with tag in gui
gui for competition
gui for searching images
gui for switching modes
gui for tagging images
gui for browsing tag score 
gui not being ugly
gui extension to thunar for tag



