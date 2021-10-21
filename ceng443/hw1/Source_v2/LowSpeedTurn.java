public class LowSpeedTurn extends TrackFeature {

  public LowSpeedTurn(int turnNo, TurnDirection direction, double distance, double roughness) {
    // Fill this method
    this.featureNo = turnNo;
    this.turnDirection = direction;
    this.distance = distance;
    this.roughness = roughness;
    this.featureMultiplier = 1.3;
  }
}