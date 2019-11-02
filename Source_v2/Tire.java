public abstract class Tire {

  protected double speed;
  protected double degradation;

  // Add to uml
  protected double tireMultiplier;

  public double getSpeed() {
    return speed;
  }

  public double getDegradation() {
    return degradation;
  }

  abstract public void tick(TrackFeature f);
}
