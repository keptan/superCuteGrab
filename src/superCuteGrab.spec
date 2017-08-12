design:
	some big giant class heirarchy with inheretence and lots of std:: stuff
	

classes:
	image/file class :: holds filename and location sort of thing, should also hold the MD5 of the image,actually requires opening the file as a stream i guess
		|_ IPTCclass :: holds IPTC data, interfaces with EXIV to write and read IPTC 
		|_ ELO class :: holds ELO score, and has methods to calculate ELO
		|_ BooruClass  :: holds xml data, interfaces with CURL to search and read boorus
						sends data back to the IPTC class for writing 

		|_  


Notes:

	what if handed a url or md5 without a file, the control flow would be backwards UP the calss heirarchy, with Netclass geetting IPTC data and eventually getting a filename. And the ELO data wouldnt come into it

	definitely generate some kind of page file to avoid searching through the filesystem constantly which is super slow, sorted by md5

	implementation

	Use the tinyxml2 and CURL to do the itnernet stuff
	gnu core utils can generate md5s for you
	navigate filesystem using experimental/filesytstem stuff
E
