public class HardTire extends Tire {

  public HardTire() {
    this.speed = 275;
    this.degradation = 0;
    this.tireMultiplier = 1.0;
  }

  public void tick(TrackFeature f) {}
}
