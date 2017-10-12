#ifndef IMAGEBASE_H
#define IMAGEBASE_H

#include "metaData.h"
#include "skillBase.h"
#include <algorithm> 

namespace cute
{
	class ImageBase
	{
		private :
			std::string loc;
			int scrolledTo;

			std::vector<MetaData> localImages;

		public :


			SkillBase *skillBase;
			FileBase *fileBase;
			std::vector<MetaData> filteredImages;
			ImageBase(std::string l,SkillBase *s,FileBase *f);
			int readDirectory(int i = 0); //different versions that only add to vector if they fulfill criteria

			int readDirectory(std::string l); //different versions that only add to vector if they fulfill criteria
			int scrollDirectory(int i);
			int filter(std::string t);
			MetaData findMatch(MetaData m,int c =0);

			std::vector<MetaData> *collectImages();
			void runElo(MetaData w,MetaData l,int t = 0);
	};
}
			
#endif
