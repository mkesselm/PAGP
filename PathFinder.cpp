#include "PathFinder.h"

/*******************************************************************************
 * enter targetloc into history's ioHistory;
 * Check StageMap index offsets--
 *  if 0
 *    if history's offsetHistory != 0
 *      set history's proximityPreference to near,
 *      empty offsetHistory,
 *      empty ioHistory
 *  else (non-0 offsets)
 *    load targetloc copy into ioHistory
 *    switch on history's proximityPreference:
 *      0: offset targetloc to closest offset
 *      1: offset targetloc to second-closest offset
 *      2: offset targetloc to farthest offset
 *    load altered targetloc copy into offsetHistory
 *    if front of offsetHistory == front-1 of offsetHistory
 *      ++hoverCount
 *    else (front and front-1 of offsetHistory are diferent)
 *      hoverCount = 0
 *    if hoverCount >= 5
 *      if history's proximityPreference == far
 *        set to near
 *      else
 *        increment
 *      while ioHistory queue !empty
 *        load front of queue into pass-vector
 *        switch (as above) on proximityPreference
 *        send offset pass-vector vector to transformOut
 *        wait until transormOut is complete
 *      offset targetloc by new proximityPreference
 *******************************************************************************
 */

void pathFinder (InputQueue* history, StageMap* stage, std::vector<int> targetloc) {
  std::vector<int> targetlocOffset(targetloc);
  int i, j, k, iO, jO, kO;
  i = targetloc.at(0);
  j = targetloc.at(1);
  k = targetloc.at(2);
  iO = stage->stagemap.at(i).at(j).at(k).xOffset;
  jO = stage->stagemap.at(i).at(j).at(k).yOffset;
  kO = stage->stagemap.at(i).at(j).at(k).zOffset;
    // we're in a free space so reset proximityPreference and dump the queue
  if(iO == 0 && jO == 0 && kO == 0) {
      if (history->offsetHistory.size() != 0) {
        history->proximityPreference = 0;
        history->offsetHistory.clear();
        while(!history->ioHistory.empty()) { // unfortunately queues are kind of feature-stupid
          history->ioHistory.pop();
        }
      }
    // we're in an occupied spot
  } else {
    history->ioHistory.push(targetloc);
    switch (history->proximityPreference) {
      case 2:
      // far proximityPreference
        if (iO < jO) {
          if (jO < kO) {
            targetlocOffset.at(2) += kO;
          } else {
            targetlocOffset.at(1) += jO;
          }
        } else if (iO < kO) {
          targetlocOffset.at(2) += kO;
        } else {
          targetlocOffset.at(0) += iO;
        }
        break;
      case 1:
      // mid proximityPreference
        if (iO < jO) {
          if (jO < kO) {
            targetlocOffset.at(1) += jO;
          } else {
            targetlocOffset.at(2) += kO;
          }
        } else if (iO < kO) {
          targetlocOffset.at(0) += iO;
        } else {
          targetlocOffset.at(2) += kO;
        }
        break;
      default:
      // default to near proximityPreference
        if (iO < jO) {
          if (iO < kO) {
            targetlocOffset.at(0) += iO;
          } else {
            targetlocOffset.at(2) += kO;
          }
        } else if (jO < kO) {
          targetlocOffset.at(1) += jO;
        } else {
          targetlocOffset.at(2) += kO;
        }
    }
    if (history->offsetHistory.back() == targetlocOffset) {
      history->hoverCount++;
    } else {
      history->hoverCount = 0;
    }
    // load this corrected vector into our offsetHistory
    history->offsetHistory.push_back(targetlocOffset);

    if (history->hoverCount >= 5) {
      if (history->proximityPreference == 2) {
        history->proximityPreference = 0;
      } else {
        history->proximityPreference++;
      }
      while (!history->ioHistory.empty()) {
        // TODO REFACTOR SWITCH TO A SEPARATE OFFSETVECTOR CLASS
        // clawOut(history->ioHistory.front());
        history->ioHistory.pop();
        //load front of queue into pass-vector
        // *        switch (as above) on proximityPreference
        // *        send offset pass-vector vector to transformOut
        // *        wait until transormOut is complete
      }
    }
  }
  // we restructure our outgoing vector to reflect any offset changes made above
  targetloc.swap(targetlocOffset);
}
