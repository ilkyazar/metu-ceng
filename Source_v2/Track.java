import java.util.ArrayList;
// my imports
import java.util.Iterator;

public class Track {

  private String trackName;
  private ArrayList<TrackFeature> featureList;
  private boolean isClockwise;

  // add to uml
  private Iterator<TrackFeature> trackIterator;

  public Track() {
    // Fill this method
    this.trackName = "";
    this.featureList = null;
    this.isClockwise = false;
    this.trackIterator = null;
  }

  public Track(String trackName, ArrayList<TrackFeature> featureList, boolean isClockwise) {
    // Fill this method
    this.trackName = trackName;
    this.featureList = featureList;
    this.isClockwise = isClockwise;
    this.trackIterator = this.featureList.iterator();
  }

  public String getTrackName() {
    return trackName;
  }

  public void setTrackName(String trackName) {
    this.trackName = trackName;
  }

  public ArrayList<TrackFeature> getFeatureList() {
    return featureList;
  }

  public void setFeatureList(ArrayList<TrackFeature> featureList) {
    this.featureList = featureList;
  }

  public boolean isClockwise() {
    return isClockwise;
  }

  public void setClockwise(boolean clockwise) {
    isClockwise = clockwise;
  }

  public int getTrackLength() {
    // Fill this method
    return this.featureList.size();
  }

  public TrackFeature getNextFeature() {
    // Fill this method
    if (this.trackIterator.hasNext()) {
      return this.trackIterator.next();
    }
    else {
      return this.featureList.get(0);
    }
  }

  public void addFeature(TrackFeature feature) {
    // Fill this method
    this.featureList.add(feature);
  }

  public boolean isValidTrack() {
    // Fill this method
    if (this.featureList.get(0).turnDirection != TurnDirection.STRAIGHT && 
        this.featureList.get(this.getTrackLength() - 1).turnDirection != TurnDirection.STRAIGHT) {
      return false;
    }      
    int rightTurns = 0, leftTurns = 0;

    for (int i = 0; i < this.getTrackLength(); i++) {
      if (this.featureList.get(i).turnDirection == TurnDirection.LEFT) {
        leftTurns++;
      }
      else if (this.featureList.get(i).turnDirection == TurnDirection.RIGHT) {
        rightTurns++;
      }  
    }
    
    if (this.isClockwise() && rightTurns - leftTurns != 4) {
      return false;
    }
    if (!this.isClockwise() && leftTurns - rightTurns != 4) {
      return false;
    }

    return true;
  }
}
