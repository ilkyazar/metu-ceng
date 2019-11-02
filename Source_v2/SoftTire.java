public class SoftTire extends Tire {

  public SoftTire() {
    this.speed = 350;
    this.degradation = 0;
    this.tireMultiplier = 1.2;
  }

  public void tick(TrackFeature f) {}
}
