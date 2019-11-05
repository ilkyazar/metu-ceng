public abstract class TrackFeature {

  protected int featureNo;
  protected TurnDirection turnDirection;
  protected double distance;
  protected double roughness;

  // New instance variable
  protected double featureMultiplier;

  // New method
  public double getFeatureMultiplier() {
    return featureMultiplier;
  }

  public int getFeatureNo() {
    return featureNo;
  }

  public double getRoughness() {
    return roughness;
  }

  public double getDistance() {
    return distance;
  }

  public TurnDirection getTurnDirection() {
    return turnDirection;
  }

}
