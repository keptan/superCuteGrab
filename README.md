Go use [megacute](https://github.com/keptan/megacute) instead, it uses [Hydrus](https://hydrusnetwork.github.io/hydrus/index.html) as the image db and is much better.

# superCuteGrab
![](https://raw.githubusercontent.com/keptan/superCuteGrab/master/example.png)
Booru image tag-fetching and ranking with Microsoft TRUESKILL and ELO.

Use the halo 2 matchmaking algorithm, ripped booru data, and manual labour to objectively rank anime images and boorutagss. 

# Ranking Details: 
Using [Microsoft Trueskill](http://www.moserware.com/2010/03/computing-your-skill.html) we assign a score and a score confidence to tags and images, unlike chess elo Trueskill can rank team games, allowing us to assign scores to booru-tags and other metadata as well as just the image MD5 itself.

![](http://www.moserware.com/assets/computing-your-skill/TrueSkillCurvesBeforeExample.png)


# Features 
* Rank images in competition
* Rip image metadata from various booru websites, use saucenao to track down tricky images and md5 mismatches
* Search images with metadata, view statistics on the most performant tags and images 




