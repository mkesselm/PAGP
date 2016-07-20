void stageMapper (StageMap* stage, MappingVector2d* xy, MappingVector2d* xz) {
  int height = xy->dimOffset.size();
  int width = xy->dimOffset.at(0).size();
  int depth = height;

  stage->stagemap.resize(height);
  for(int i = 0; i <= (height - 1); i++) {
    stage->stagemap.at(i).resize(width);
    for(int j = 0; j <= (width - 1); j++) {
      stage->stagemap.at(i).at(j).resize(depth);
      for(int k = 0; k <= (depth - 1); k++) {
        stage->stagemap.at(i).at(j).at(k).xOffset = max(xy->dimOffset.at(i).at(j).xOffset, xz->dimOffset.at(i).at(j).xOffset);
        stage->stagemap.at(i).at(j).at(k).yOffset = xy->dimOffset.at(i).at(j).yzOffset;
        stage->stagemap.at(i).at(j).at(k).zOffset = xz->dimOffset.at(i).at(k).yzOffset;
      }
    }
  }

  // Clean-up phase--xy and xz are no longer needed now that we have our 3D MappingVector2d
  delete xy;
  delete xz;
}
