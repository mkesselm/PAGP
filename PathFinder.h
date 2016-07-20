// proximityPreference is an int rather than an enum for ease of access later on
struct InputQueue {
  int hoverCount;
  int proximityPreference; //0 - near; 1 - mid; 2 - far;
  std::queue<std::vector<int> > ioHistory;
  std::vector<std::vector < int> > offsetHistory;
};

/******************************************************************************/
void pathFinder (InputQueue*, StageMap*, std::vector<int>);
