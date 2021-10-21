public abstract class Tire {

  protected double speed;
  protected double degradation;

  // New instance variable
  protected double tireMultiplier;

  public double getSpeed() {
    return speed;
  }

  public double getDegradation() {
    return degradation;
  }

  // In order to not to duplicate the code in HardTire, SoftTire and MediumTire, I changed the tick method as not abstract.
  // abstract public void tick(TrackFeature f);

  public void tick(TrackFeature f) {
    this.degradation += f.getFeatureMultiplier() * f.getRoughness() * this.tireMultiplier;
    if (this.speed >= 100) {
      this.speed -= Math.min(75, this.degradation) * 0.25;
    }
  }

}
