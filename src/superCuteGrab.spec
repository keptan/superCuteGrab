design:
	some big giant class heirarchy with inheretence and lots of std:: stuff
	

classes:
	image/file class :: holds filename and location sort of thing
		|_ ELO class :: holds ELO score, and has methods to calculate ELO
		|_ IPTCclass :: holds IPTC data, interfaces with EXIV to write and read IPTC 
		|_ NetClass  :: holds xml data, interfaces with CURL to search and read boorus
						sends data back to the IPTC class for writing 

		|_  


Notes:

	what if handed a url or md5 without a file, the control flow would be backwards UP the calss heirarchy, with Netclass geetting IPTC data and eventually getting a filename. And the ELO data wouldnt come into it

	implementation
